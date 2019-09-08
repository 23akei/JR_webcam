#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(){
	cv::Mat src_img = cv::imread("circle-1.png");
	imshow("Hoge", src_img);
	cv::waitKey(0);
	return 0;
}
