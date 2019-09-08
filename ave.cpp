/*
 * webカメラ映像からリアルタイムで円を検出
 * 特定色を抽出し、その中から検出
 * 
 * ＢＧＲ画像から特定色抽出
 * 抽出画像の平滑化
 * 平滑化画像から円を検出
 * camfindcir拡張
 * 
 * 数フレームの平均を取る
 * 各フレームで検出された円の平均を取る
 */

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
//using namespace std;

//Window name
#define WINNAME "OUTPUT"

//平均を取るフレームの数
#define LOOP 3

//upper limit and lower limit of color
#define B_MIN 156
#define G_MIN 127
#define R_MIN 127
#define B_MAX 255
#define G_MAX 255
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
    while (1){
        Mat img_in;
        std::vector<Vec3f>::iterator it[LOOP], out;
        std::vector<cv::Vec3f> circles[LOOP];
        for(int i = 0; i < LOOP; i++){
            Mat img_mask, img_pick, img_gray, img_gauss;
            webc >> img_in;
        
            //Pick up
            inRange(img_in, s_min, s_max, img_mask);
            img_in.copyTo(img_pick, img_mask);

            //二値化　平滑化
            GaussianBlur(img_pick, img_gauss, Size(5, 5), 2, 5);
            cvtColor(img_gauss, img_gray, CV_BGR2GRAY, 0);
            adaptiveThreshold(img_gray, img_gray, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 3, 2);

            // Hough変換による円の検出と検出した円の描画
            HoughCircles(img_gray, circles[i], CV_HOUGH_GRADIENT, 1, img_gray.rows/4, 20, 50);

            //円の描画
            it[i] = circles[i].begin();
            for(; it[i]!=circles[i].end(); ++it[i]) {
                Point center(saturate_cast<int>((*it[i])[0]), saturate_cast<int>((*it[i])[1]));
                int radius = saturate_cast<int>((*it[i])[2]);
                circle(img_in, center, radius, cv::Scalar(0,255,255), 2);

                printf("center:x=%d,\t y=%d\tradius: %d\n",center.x, center.y, radius);  //debug        
            }

            printf("----------\n"); //debug
        }
        //itをソートして平均をとって、outに返す
        
        /*
        //debug
        for(int i = 0; i < LOOP; i++){
            for(; it[i] != circles[LOOP].end(); ++it[i])
                printf("%d,%d\t ", (int)circles[LOOP][i][0], (int)circles[LOOP][i][1]);
            printf("\n");
        }
        */
        
        imshow(WINNAME, img_in);    //output
        if(waitKey(30) > 0) break;
    }
    return 0;
}