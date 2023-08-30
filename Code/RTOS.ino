#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>


 
LiquidCrystal_I2C I2C_LCD1(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const int flamePin = 4;
const int BuzzerPin = 2;
const int ledPin = 13;  
const int IrPin = 35;  
const int gasPin = 34;  

bool interruptOccurred = false;

Servo GasServo;
const int servoPin = 5;


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

  pinMode(flamePin, INPUT_PULLUP);
  pinMode(gasPin, INPUT);
  pinMode(IrPin, INPUT);
  pinMode(BuzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  GasServo.attach(servoPin);
  GasServo.write(0);

  xTaskCreate(task1, "Task 1", 10000, NULL, 1, NULL);
  xTaskCreate(task2, "Task 2", 10000, NULL, 2, NULL);
}

void loop() {
  
}

String getPassword() {
  GasDetection();
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

void flameDetected(){
  interruptOccurred = true;
}

void flameDetection(){
  int flameValue = analogRead(flamePin);
  
  if (flameValue < 500) {
    Serial.println("Flame detected!");
    I2C_LCD1.print("Flame detected!");
    alarm();
    I2C_LCD1.clear(); 
  }
}

void GasDetection(){
  int gasValue = analogRead(gasPin);

  if (gasValue > 2000) {
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



#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

void task1(void *parameter) {
  while (1) {
    flameDetection();
    GasDetection();
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 1 second
  }
}

void task2(void *parameter) {
  while (1) {
  int IrValue = analogRead(IrPin);

  if(IrValue < 2000){
    String password = getPassword(); // in loop
    if(password == "1234"){
      String amount = getPassword();
      GasServo.write(180);
      int T = amount.toInt()*0.5;
      Serial.println(T);
      delay(T*1000);
      GasServo.write(0);
      delay(4000);
    }
  }
    vTaskDelay(500 / portTICK_PERIOD_MS); // Delay for 0.5 seconds
  }
}