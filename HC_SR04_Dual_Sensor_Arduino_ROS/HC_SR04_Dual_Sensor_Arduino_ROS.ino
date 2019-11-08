/*
   Title: HC-SR04 Dual Sensor Arduino ROS
   Author: Muhammad Irsyad Sahalan
   Note: This sketch is an example of 2 units of HC-SR04 Ranging Module connected to Arduino Uno ATMega 238.
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
     
*/

#include <ros.h>
#include <ros/time.h>
#include <std_msgs/Float32.h>

#define MAX_RANGE 4.0
#define MIN_RANGE 0.02

ros::NodeHandle  nh;

std_msgs::Float32 range_msg_0;  //instantiate message for sensor 1
ros::Publisher pub_range_0( "/ultrasound_0", &range_msg_0); //  instantiate publisher for sensor 1
std_msgs::Float32 range_msg_1;  //instantiate message for sensor 2
ros::Publisher pub_range_1( "/ultrasound_1", &range_msg_1); //  instantiate publisher for sensor 2

float range_0, range_1;
unsigned long duration_0, duration_1;
bool prev_state_0, prev_state_1;

ISR (PCINT2_vect)
// handle pin change interrupt for D0 to D7 here
{
  static unsigned long start_time_0, start_time_1;

  if (PIND & bit (3))  // if high input at 3
  {
    if (!(prev_state_0)) //if  previous pin state was low
    {
      start_time_0 = micros();  //take micros value of the start time
      prev_state_0 = true;
    }
  }
  else
  {
    if (prev_state_0) //if  previous pin state was high
    {
      duration_0 = micros() - start_time_0; //get the duration of the echo pulse
      prev_state_0 = false;
    }
  }

  if (PIND & bit (4)) // if high input at 4
  {
    if (!(prev_state_1)) //if  previous pin state was low
    {
      start_time_1 = micros();  //take micros value of the start time
      prev_state_1 = true;
    }

  }
  else
  {
    if (prev_state_1) //if  previous pin state was high
    {
      duration_1 = micros() - start_time_1; //get the duration of the echo pulse
      prev_state_1 = false;
    }

  }
  
  /* If additional HC-SR04 sensor to be include, copy the same control statement and
   *  change bit number accordingly and create another start_time_, prev_state_, and duration_
  */
  
}  // end of PCINT2_vect

void setup() {
  nh.initNode();
  nh.advertise(pub_range_0);
  nh.advertise(pub_range_1);

  // pin change interrupt
  PCMSK2 |= bit (PCINT19);  // want pin 3
  PCMSK2 |= bit (PCINT20);  // want pin 4
  //If additional HC-SR04 sensor to be included from D0 to D7 pin, copy the above commmand
  // and change PCINT<value> between 16 to 23
  
  PCIFR  |= bit (PCIF2);    // clear any outstanding interrupts
  PCICR  |= bit (PCIE2);    // enable pin change interrupts for D0 to D7 @ 0b00000100

  pinMode (2, OUTPUT);
  pinMode (3, INPUT_PULLUP);
  pinMode (4, INPUT_PULLUP);
  //If additional HC-SR04 sensor to be included, add another pinMode()
}

void loop()
{
  digitalWrite(2, LOW);
  delayMicroseconds(2);
  digitalWrite(2, HIGH);  // transmit a trigger pulse in 10 microseconds
  delayMicroseconds(10);
  digitalWrite(2, LOW);
  delay(5); // provide some delay before the next trigger pulse
  
  range_msg_0.data = range(duration_0);
  range_msg_1.data = range(duration_1);
  pub_range_0.publish(&range_msg_0);
  pub_range_1.publish(&range_msg_1);
  nh.spinOnce();
}

float range(unsigned long duration)
//publish only reading within MIN_RANGE and MAX_RANGE
{
  float val = (float(duration) / 58) / 100;
  if (val >= MIN_RANGE && val <= MAX_RANGE)
    return val;
  else
    return 0.0;
}
