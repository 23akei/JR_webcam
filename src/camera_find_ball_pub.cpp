//Find a ball and publish the data where it is.

#include <ros/ros.h>
#include <std_msgs/Float64MultiArray.h>
#include <sstream>
#include "Particle.hpp"


int main(int argc, char **argv){

    //ROS initialized
    ros::init(argc, argv, "point_publisher_node");
    ros::NodeHandle nh;
    ros::Publisher ball_pub = nh.advertise<std_msgs::Float64MultiArray>("position_of_ball", 10);

    //OpenCV initalized
    cv::VideoCapture video;
    video.open(-1);     //id of camera (-1 is default)

    if (!video.isOpened()){
        std::cout << "can't open your video" << std::endl;
    }

    bool start = false;
    std_msgs::Float64MultiArray point;
    point.data.resize(2);

    ParticleFilter *pf = new ParticleFilter();

    ros::Rate looprate(10);
    while (ros::ok()){      //Main loop
        cv::Mat frame;
        video >> frame;

        //終了判定の条件
        if (frame.empty()){
            break;
        }

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
        ball_pub.publish(point);
        ros::spinOnce();

        looprate.sleep();

        //for debug
        cv::imshow("Camera", frame);
        char c = (char)cv::waitKey(1);
        if (c == 'q')
            break;
    }

    return 0;
}