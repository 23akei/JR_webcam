/*
 * webカメラ映像からリアルタイムでボールを検出
 * 
 * ＢＧＲ画像から特定色抽出
 * 抽出画像の平滑化
 * 平滑化画像から円を検出
 */

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
//using namespace std;

//Window name
#define WINNAME "OUTPUT"

//upper limit and lower limit of color
#define B_MIN 0
#define G_MIN 0
#define R_MIN 127
#define B_MAX 32
#define G_MAX 32
#define R_MAX 255

int main(int argc, char** argv)
{
    //Open the video
    VideoCapture webc(argc < 2 ? -1 : atoi(argv[1]));
    if(!webc.isOpened()){
        fprintf(stderr, "cannnot open the camera.\n");
        return -1;
    }

    //Define
    
    //namedWindow(WINNAME, CV_WINDOW_AUTOSIZE);
    //namedWindow("DEBUG", CV_WINDOW_AUTOSIZE);
    Scalar s_min = Scalar(B_MIN, G_MIN, R_MIN);
    Scalar s_max = Scalar(B_MAX, G_MAX, R_MAX);
    
    //main loop
    while(1){
        Point center;
        Mat img_in, img_mask, img_pick, img_gray, img_gauss, img_out;
        webc >> img_in;
        
        //Pick up
        inRange(img_in, s_min, s_max, img_mask);
        img_in.copyTo(img_pick, img_mask);

        //二値化　平滑化
        GaussianBlur(img_pick, img_gauss, Size(5, 5), 2, 5);
        cvtColor(img_gauss, img_gray, CV_BGR2GRAY, 0);
        adaptiveThreshold(img_gray, img_gray, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 3, 2);

        //特定色のピクセルの検出
        int np = 0;
        for(int y = 0; y < img_gray.rows; y++){
            for(int x = 0; x < img_gray.cols; x++){
                if(img_gray.at<Vec3b>(y, x)[0] == 0 && 
                img_gray.at<Vec3b>(y, x)[1] == 0 &&
                img_gray.at<Vec3b>(y, x)[2] == 0){
                    center.x += x;
                    center.y += y;
                    np++;
                }
            }
        }
        center.x /= np;
        center.y /= np;
        circle(img_gray, center, 5, (255, 0, 0), -1);

        printf("center:%d, %d----np = %d------\n", center.x, center.y, np); //debug

        imshow(WINNAME, img_gray); //output
        if(waitKey(30) > 0) break;
    }

    return 0;
}