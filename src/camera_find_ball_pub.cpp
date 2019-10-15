//Find a ball and publish the data where it is.
//パーティクルフィルタで探すモードと画像の画素数で探すモードを備える

//画像処理のモード
//0:なし
//1:パーティクルフィルタ
//2:画素数による検知

#include <ros/ros.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/Int32.h>
#include <sstream>
#include "Particle.hpp"

//upper limit and lower limit of color
#define B_MIN 96
#define G_MIN 10
#define R_MIN 10
#define B_MAX 255
#define G_MAX 127
#define R_MAX 127

int smode = 0;  //動作ステータス(画像処理のモード)
std_msgs::Int32 px; //指定色のピクセル数

void getmode(std_msgs::Int32 status);   //動作ステータス更新

//画像処理関数
std_msgs::Float64MultiArray particle(cv::Mat frame);
std_msgs::Float64MultiArray mask(cv::Mat frame);


int main(int argc, char **argv){
    ros::init(argc, argv, "point_publisher_node");
    ros::NodeHandle nh;
    ros::Publisher ball_pub = nh.advertise<std_msgs::Float64MultiArray>("position_of_ball", 10);    //検出したものの重心
    ros::Publisher pixel_col = nh.advertise<std_msgs::Int32>("pixel", 10);  //特定色の画素数を出力
    ros::Subscriber mode_sub = nh.subscribe<std_msgs::Int32>("search_mode", 10, getmode);   //モード変更

    px.data = 0;

    //OpenCV initalized
    cv::Mat img;
    cv::VideoCapture video;
    video.open(-1);     //id of camera (-1 is default)

    if (!video.isOpened()){
        std::cout << "can't open your video" << std::endl;
    }

    std_msgs::Float64MultiArray point;
    point.data.resize(2);


    ros::Rate looprate(10);
    while (ros::ok()){      //Main loop
        video >> img;
        if(smode == 1 || smode == 0)
            point = particle(img);
        if(smode == 2){
            point = mask(img);
            pixel_col.publish(px);
        }
        ball_pub.publish(point);
        ros::spinOnce();
        looprate.sleep();

        /*//終了判定の条件
        if (img.empty()){
            break;
        }*/      
    }

    return 0;
}

void getmode(std_msgs::Int32 status){   //動作ステータスの更新
    smode = status.data;
}

std_msgs::Float64MultiArray particle(cv::Mat frame){
    bool start = false;
    ParticleFilter *pf = new ParticleFilter();
    std_msgs::Float64MultiArray point;
    point.data.resize(2);
    if (!start)
    {
        std::vector<int> upper = { frame.size().width, frame.size().height, 10, 10 };
        std::vector<int> lower = { 0, 0, -10, -10 };
        std::vector<int> noise = { 30, 30, 10, 10 };

        pf = new ParticleFilter(300, frame.size().height, frame.size().width, upper, lower, noise);
        start = true;
    }

    pf->Resampling();
    pf->Predict();
    pf->CalcWeight_b(frame);

    Particle p = pf->Measure();
    p.PrintParameter();

    cv::Point pt = cv::Point(p.width, p.height);

    //全部の点を表示
    std::vector<Particle> particle = pf->GetPaticleVector();
    for (int i = 0; i < particle.size(); ++i)
    {
        cv::Point pp = cv::Point(particle[i].width, particle[i].height);
        cv::circle(frame, pp, 1, cv::Scalar(0, 255, 255), -1);
    }

    //中心を赤色で表示
    cv::circle(frame, pt, 3, cv::Scalar(0, 0, 255), -1);

    //publish the data
    point.data[0] = pt.x;
    point.data[1] = pt.y;

    //for debug
    /*cv::imshow("Camera", frame);
    char c = (char)cv::waitKey(1);
    if (c == 'q')
        break;*/

    return point;
}

std_msgs::Float64MultiArray mask(cv::Mat img_in){
    px.data = 0;
    std_msgs::Float64MultiArray pt;
    pt.data.resize(2);

    cv::Scalar s_min = cv::Scalar(B_MIN, G_MIN, R_MIN);
    cv::Scalar s_max = cv::Scalar(B_MAX, G_MAX, R_MAX);
    
    cv::Point center;
    cv::Mat img_mask, img_pick, img_gray, img_gauss;
    
    //Pick up
    cv::inRange(img_in, s_min, s_max, img_mask);
    img_in.copyTo(img_pick, img_mask);

    //二値化　平滑化
    cv::GaussianBlur(img_pick, img_gauss, cv::Size(5, 5), 2, 5);
    cv::cvtColor(img_gauss, img_gray, CV_BGR2GRAY, 0);
    cv::adaptiveThreshold(img_gray, img_gray, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 3, 2);

    //特定色のピクセルの検出
    for(int y = 0; y < img_gray.rows; y++){
        for(int x = 0; x < img_gray.cols; x++){
            if(img_gray.at<cv::Vec3b>(y, x)[0] == 0 && 
            img_gray.at<cv::Vec3b>(y, x)[1] == 0 &&
            img_gray.at<cv::Vec3b>(y, x)[2] == 0){
                center.x += x;
                center.y += y;
                px.data++;
            }
        }
    }

    //for debug
    //cv::imshow("win", img_pick);

    pt.data[0] = center.x / px.data;
    pt.data[1] = center.y / px.data;
    return pt;
}
