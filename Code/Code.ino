#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>
#include <WiFi.h>
#include <Firebase.h>
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"
#define WIFI_SSID "Shrif"
#define WIFI_PASSWORD "SHsh1212*"

#define API_KEY "AIzaSyCDIX_8R5TSoMmUHrZVrufLrAC32vzNUnw"
#define USER_EMAIL "mshmohsen2004@gmail.com"
#define USER_PASSWORD "123456"
#define DATABASE_URL "https://odda-5c342-default-rtdb.firebaseio.com/"

FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig firebaseConfig;
FirebaseConfig config;
FirebaseData fbdo;
String uid ; 

String databasePath;
int irsensor;
String amountPath = "/amount";
String timePath = "/timestamp";
String parentPath;

int timestamp;

int counter = 0;
FirebaseJson json;
unsigned long sendDataPrevMillis = 0;
const unsigned long timerDelay = 5000; // Delay between sending data (milliseconds)

void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}


void initFirebase() {
  firebaseConfig.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.begin(&firebaseConfig, &auth);
  Firebase.reconnectWiFi(true);
}

unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    // Serial.println("Failed to obtain time");
    return (0);
  }
  time(&now);
  return now;
}

 
LiquidCrystal_I2C I2C_LCD1(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const int flamePin = 4;
const int BuzzerPin = 2;
const int ledPin = 13;  
const int IrPin = 35;  
const int gasPin = 34;  

Servo GasServo;
const int servoPin = 5;
const char* ntpServer = "pool.ntp.org";

const byte ROWS = 3;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'}
};
byte rowPins[ROWS] = {32,33,25};
byte colPins[COLS] = {27,14,12};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
 
void setup()
{
  Serial.begin(115200);
  // Initialize The I2C LCD
  I2C_LCD1.init();
  // Turn ON The Backlight
  I2C_LCD1.backlight();

  pinMode(flamePin, INPUT);
  pinMode(gasPin, INPUT);
  pinMode(IrPin, INPUT);
  pinMode(BuzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  GasServo.attach(servoPin);
  GasServo.write(0);
  initWiFi();
  initFirebase();
  //  // Assign the user sign in credentials
  configTime(0, 0, ntpServer);
  // Assign the api key (required)
   config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
   auth.user.password = USER_PASSWORD;
  // // Assign the RTDB URL (required)
   config.database_url = DATABASE_URL;
   Firebase.reconnectWiFi(true);
   fbdo.setResponseSize(4096);
  // // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback;  //see addons/TokenHelper.h
  config.max_token_generation_retry = 5;
  // // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);
  // Serial.println(String(Firebase.getToken()));
  // Update database path
  databasePath = "/UsersData/" + uid + "/readings";
}



void flameDetection(){
  int flameValue = analogRead(flamePin);
  
  if (flameValue < 200) {
    Serial.println("Flame detected!");
    I2C_LCD1.print("Flame detected!");
    alarm();
    I2C_LCD1.clear(); 
  }
}

void GasDetection(){
  int gasValue = analogRead(gasPin);
  // Serial.println(gasValue);

  if (gasValue > 1000) {
    Serial.println("Gas detected!");
    I2C_LCD1.print("Gas detected!");
    alarm();
    I2C_LCD1.clear(); 
  }
}

void alarm(){
  for(int i = 0 ; i < 3 ; i++){
    digitalWrite(BuzzerPin,HIGH);
    digitalWrite(ledPin, HIGH);
    delay(500); 
    digitalWrite(BuzzerPin,LOW);
    digitalWrite(ledPin, LOW);
    delay(500); 
  }  
}



String getInput() {
  // flameDetection();
  // GasDetection();
  String password = "";
  char key = keypad.getKey();
  
  while (key != '9') {
    if (key) {
      password += key;
      Serial.print(key);
    }
    key = keypad.getKey();
  }
  Serial.println();
  return password;
}

void loop() {
  unsigned long currentMillis = millis();
  flameDetection();
  GasDetection();
  int IrValue = analogRead(IrPin);

  if (IrValue < 2000) {
    I2C_LCD1.print("Enter Password");
    String password = getInput();
    I2C_LCD1.clear();
    if (password == "1234") {
    I2C_LCD1.print("Enter Amount");
      String amountStr = getInput();
    I2C_LCD1.clear();
      int amount = amountStr.toInt(); // Convert amountStr to an integer
      GasServo.write(180);
      int T = amount*0.5;
      Serial.println(T);
      delay(T*1000);
      GasServo.write(0);
      
      
      if (Firebase.ready() && (currentMillis - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0) && amount != 0) {
        sendDataPrevMillis = currentMillis;
        // Get current timestamp
        timestamp = getTime();
        
      parentPath = "/gaslogs/" + String(uid) + "/readings/" + String(timestamp);
     String Path = "/gaslogs/" + String(uid) + "/readings/" + String(counter-1);

        json.clear(); // Clear the JSON object before using it
        json.set(amountPath.c_str(), amount); // Set gas sensor reading

        // Add other data to the JSON object if needed
        json.set(timePath, String(timestamp));
        
        if (Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json)) {
          Serial.println("Data sent to Firebase successfully");
        } else {
          Serial.print("Error sending data to Firebase: ");
          Serial.println(fbdo.errorReason());
        }
        
        delay(5000);
        
  if (Firebase.RTDB.getInt(&firebaseData, String(Path + "/amount")) && amount == 0) {
  int getGasValue = firebaseData.to<int>();
  Serial.println(getGasValue);
      GasServo.write(180);
      int T = getGasValue * 0.5;
      Serial.println(T);
      delay(T*1000);
      GasServo.write(0);
  Serial.print("Received Gas Value from Firebase: ");
  Serial.println(getGasValue);
} else {
  Serial.print("Error reading Gas Value from Firebase: ");
  Serial.println(firebaseData.errorReason());
}
        
        Serial.println("-------------------------------------");
      }
      counter++;
    }
  }
}
