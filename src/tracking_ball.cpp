//subuscribe the point of the ball and tracking it

#include <ros/ros.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/Int32.h>
#include <geometry_msgs/Twist.h>

#define SPD (1.0)
#define FRAME_WIDTH (800)
#define FRAME_CENTER (100)  //中央とみなす範囲。中心からプラスマイナスこれだけ。

//購読時コールバック関数
void slide(std_msgs::Float64MultiArray position);
void track(std_msgs::Int32 touch);

int status = 0;

ros::NodeHandle nh;
ros::Subscriber get_position = nh.subscribe<std_msgs::Float64MultiArray>("position_of_ball", 10, slide);
ros::Subscriber touch_sensor = nh.subscribe<std_msgs::Int32>("touch_sensor", 10, track);
ros::Publisher tracking = nh.advertise<geometry_msgs::Twist>("sub", 1);


int main(int argc, char **argv){
    ros::init(argc, argv, "tracking_ball_node");
    
    ros::spin();
    
    return 0;
}

void slide(std_msgs::Float64MultiArray position){
    if(status == 1){
        geometry_msgs::Twist twist;
        if(position.data[0] < FRAME_WIDTH/2 - FRAME_CENTER){//left
            twist.linear.x = -SPD;
            tracking.publish(twist);
        }else if(position.data[0] > FRAME_WIDTH/2 + FRAME_CENTER){//right
            twist.linear.x = SPD;
            tracking.publish(twist);
        }else{//center
            twist.linear.y = SPD;
            tracking.publish(twist);
        }
    }
    ros::spin();
}

void track(std_msgs::Int32 touch){
    if(touch.data == 0) status = 1;
    else if(touch.data == 1) status = 0;
}

