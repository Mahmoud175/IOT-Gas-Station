# IOT-Gas-Station
Smart Gas Station Project Using Esp32 Arduino



## Brief description of the project :
This project is a gas station system that utilizes three sensors: the IR sensor, the flame sensor, and the gas sensor. When a car is detected by the IR sensor, it is prompted to input a preset password (1234) via a keypad. If correct, the car can enter and specify the amount of fuel liters it requires, also entered via the keypad. Once the amount is known, it is sent to the Firebase and the servo motor opens to dispense the fuel until the car is filled with the desired amount. The servo motor then closes, and the car can leave. 

The flame and gas sensors are utilized to detect fire and gas leaks. If either of these occurs, the LED and buzzer will activate, lighting up and sounding an alarm. The system also includes an LCD screen that displays "Gas detected" or "Flame detected" in the event of a gas leak or fire, respectively. Additionally, the LCD screen confirms whether the password entered at the beginning was correct or incorrect and displays the amount of liters requested by the car.
