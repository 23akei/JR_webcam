//subuscribe the point of the ball and tracking it
//ボールを見失った時の挙動を指示する
//ボールを見失ったという判定はパーティクルフィルタで行う
//最初は後退しながら目的の画素で探し、発見後パーティクルフィルタで追跡
//画像処理はposition_of_ballトピックから入手

#include <ros/ros.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/Int32.h>
#include <geometry_msgs/Twist.h>

#define SPD (1.0)
#define SIKII (3000)
#define FRAME_WIDTH (640)
#define FRAME_HEIGHT (480)
#define FRAME_CENTER (100)  //中央とみなす範囲。中心からプラスマイナスこれだけ。

int margin = 20;    //端の検出の余裕
std_msgs::Float64MultiArray pos;

//購読時コールバック関数
void repos(std_msgs::Float64MultiArray position);
void numpx(std_msgs::Int32 np);

int status = 0;

ros::NodeHandle nh;
ros::Publisher img_mode = nh.advertise<std_msgs::Int32>("search_mode", 1); //画像処理のモードを指定
ros::Subscriber get_position = nh.subscribe<std_msgs::Float64MultiArray>("position_of_ball", 10, repos);
ros::Subscriber get_numpx = nh.subscribe<std_msgs::Int32>("pixel", 10, numpx);  //ピクセル数
ros::Publisher tracking = nh.advertise<geometry_msgs::Twist>("sub", 1);


int main(int argc, char **argv){
    geometry_msgs::Twist twist;
    std_msgs::Int32 sts;
    sts.data = 0;
    pos.data.resize(2);

    ros::init(argc, argv, "tracking_ball_node");
    ros::Rate lpl(10);
    while(ros::ok()){
        if(status == 1){    //後退しつつボール探索
            twist.linear.y = -SPD;
            tracking.publish(twist);
        }
        if(status == 2){    //前進しながら追跡
            twist.linear.y = SPD/2;

            //重心の位置によって左右を制御
            if(pos.data[0] > FRAME_CENTER + FRAME_WIDTH/2 && pos.data[0] < FRAME_WIDTH - margin){
                twist.linear.x = SPD;
            }
            if(pos.data[0] < FRAME_WIDTH/2 - FRAME_CENTER && pos.data[0] > margin){
                twist.linear.y - -SPD;
            }
            //中央にボールが来た場合、通常制御に戻す
            if(pos.data[0] > FRAME_WIDTH/2 - FRAME_CENTER && pos.data[0] < FRAME_WIDTH/2 + FRAME_CENTER){
                status = 0;
            }
        }

        sts.data = status;
        img_mode.publish(sts);
        ros::spinOnce();
    }
    
    return 0;
}

void repos(std_msgs::Float64MultiArray position){
    pos = position;
}

void numpx(std_msgs::Int32 np){
    if(np.data > SIKII) status = 1;
    if(np.data < SIKII) status = 2;
}