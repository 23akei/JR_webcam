#include <ros.h>
#include <ArduinoHardware.h>
#include <std_msgs/Int32.h>

#define SW 5

ros::NodeHandle nh;
std_msgs::Int32 touch;
ros::Publisher chatter("touch_sensor", &touch);

void setup() {
  // put your setup code here, to run once:
  nh.initNode();
  nh.advertise(chatter);
  pinMode(SW, INPUT);
  //Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(int(touch.data));
  touch.data = (digitalRead(SW) == HIGH)? 1 : 0;
  chatter.publish(&touch);
  nh.spinOnce();
  delay(10);
}
