//mist(array2) to OpenCV(Mat) converter

#include <mist/mist.h>
#include <mist/io/image.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

typedef mist::rgb<unsigned char> value_type;
typedef mist::array2<value_type> image_type;

cv::Mat mistCvtMat(image_type mistimg){
    cv::Mat cvimage(mistimg.height(), mistimg.width(), CV_8UC3);
    
    for(int x = 0; x < mistimg.width(); x++){
        for(int y = 0; y < mistimg.height(); y++){
            cv::Vec3b p;
            p.val[0] = mistimg(x, y).r;
            p.val[1] = mistimg(x, y).g;
            p.val[2] = mistimg(x, y).b;
            cvimage.at<cv::Vec3b>(y, x) = p;
        }
    }
    return cvimage;
}


image_type matCvtMist(cv::Mat cvimage){
    image_type mistimg(cvimage.cols, cvimage.rows).fill(255);
    
    for(int x = 0; x < cvimage.cols; x++){
        for(int y = 0; y < cvimage.rows; y++){
            mistimg(x, y).r = cvimage.at<cv::Vec3b>(y, x)[0];
            mistimg(x, y).g = cvimage.at<cv::Vec3b>(y, x)[1];
            mistimg(x, y).b = cvimage.at<cv::Vec3b>(y, x)[2];
        }
    }
    return mistimg;
}