"HR_SC04_Dual_Sensor_Arduino_ROS.ino" is an example of 2 units of HC-SR04 Ranging Module connected to Arduino Uno ATMega 238.
The dual HC-SR04 trigger by the same output pin 2 and echo at pin 3 (PCINT19) and 4 (PCINT20). It utilises
Pin Change Interrupts feature of ATMega 238. 
The distance values are then publish on two seperate <std_msgs/Float32> ROS messages.
   
   HC-SR04 info:
     Power Supply :+5V DC
     Quiescent Current : <2mA
     Working Currnt: 15mA
     Effectual Angle: <15°
     Ranging Distance : 2cm – 400 cm/1" ­ 13ft
     Resolution : 0.3 cm
     Measuring Angle: 30 degree
     Trigger Input Pulse width: 10uS
     Dimension: 45mm x 20mm x 15mm
