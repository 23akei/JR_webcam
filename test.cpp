#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/opencv.hpp"


int main(int argc, const char** argv){
	cv::VideoCapture cap0((argc < 2)? -1 : atoi(argv[1]));
	if(cap0.isOpened()) printf("the camera suceessfily opened\n");
	else{printf("cannnot open the camera\n"); return 1;}
	cv::namedWindow("image0", cv::WINDOW_AUTOSIZE);
	cv::Mat frame0;
	for(;;){
		cap0 >> frame0;
		cv::imshow("image0", frame0);
		if(cv::waitKey(30) >= 0) break;
		}
	return 0;
}

