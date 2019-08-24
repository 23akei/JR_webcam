#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

//the file name of the image
#define FNAME "rgb.jpg"
//upper limit and lower limit of color
#define B_MIN 0
#define G_MIN 0
#define R_MIN 127
#define B_MAX 127
#define G_MAX 255
#define R_MAX 255

int main(){
    //Open the image
    Mat inputimage = imread(FNAME, CV_LOAD_IMAGE_COLOR);
    if(inputimage.empty()){ fprintf(stderr, "cannot open the image.\n"); return -1;}
    
    //filtering by inRange
    Mat output_img, mask_img;
    Scalar s_min = Scalar(B_MIN, G_MIN, R_MIN);
    Scalar s_max = Scalar(B_MAX, G_MAX, R_MAX);
    inRange(inputimage, s_min, s_max, mask_img);

    //filtering from masked image
    inputimage.copyTo(output_img, mask_img);

    //Save the filered images
    imwrite("maskedimage.jpg", mask_img);
    imwrite("output.jpg", output_img);

    return 0;
}