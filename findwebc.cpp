/*
 *  find circle from Web camera.
 */

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

//upper limit and lower limit of color
#define H_MIN 160
#define S_MIN 50
#define V_MIN 50
#define H_MAX 180
#define S_MAX 255
#define V_MAX 255

int main(int argc, char **argv){
    
    cv::VideoCapture vi(argc < 2 ? -1 : atoi(argv[1]));
    if(vi.isOpened())   printf("The camera was successfuly opened.\n");
    else{fprintf(stderr, "cannot open the camera,\n"); return -1;}

    cv::namedWindow("RGB", cv::WINDOW_AUTOSIZE);
    //cv::namedWindow("HSV", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("GRAY", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("OUT", cv::WINDOW_AUTOSIZE);

    cv::Mat img_in, img_hsv,img_mask, img_mask2,img_gray, img_out, img_gauss;

    while(1){
        vi>>img_in;
        cv::cvtColor(img_in, img_gray, CV_BGR2GRAY, 0);
        cv::GaussianBlur(img_gray, img_gauss, cv::Size(11,11), 2, 2);
        
        // Hough変換による円の検出と検出した円の描画
        std::vector<cv::Vec3f> circles;
        cv::HoughCircles(img_gauss, circles, CV_HOUGH_GRADIENT, 1, img_gray.rows/4, 20, 50);

        std::vector<cv::Vec3f>::iterator it = circles.begin();
        for(; it!=circles.end(); ++it) {
            cv::Point center(cv::saturate_cast<int>((*it)[0]), cv::saturate_cast<int>((*it)[1]));
            int radius = cv::saturate_cast<int>((*it)[2]);
            cv::circle(img_out, center, radius, cv::Scalar(0,255,255), 2);
        }
        //filtering
        //cv::Scalar s_min = cv::Scalar(H_MIN, S_MIN, V_MIN);
        //cv::Scalar s_max = cv::Scalar(H_MAX, S_MAX, V_MAX);
        //cv::inRange(img_in, s_min, s_max, img_mask);
        //img_hsv.copyTo(img_hsv, img_mask);

        //以下二行がおかしい（グレースケール化がうまく行かない、実行時浮動小数点例外とエラーが出る）
        //cv::inRange(img_gray, cv::Scalar(160, 0, 0), cv::Scalar(180, 255, 255), img_mask2);
        //img_gray.copyTo(img_out, img_mask2);

        //show
        cv::imshow("RGB", img_in);
        //cv::imshow("HSV", img_hsv);
        cv::imshow("GRAY(GAUSS)", img_gauss);
        cv::imshow("OUT", img_out);
        if(cv::waitKey(30) >= 0) break;
    }
    cv::destroyAllWindows();
    return 0;
}