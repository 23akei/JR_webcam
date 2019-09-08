#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

int
main(int argc, char *argv[])
{
cv::VideoCapture cap0(1);
if(cap0.isOpened()) printf("the camera suceessfily opened\n");
else{printf("cannnot open the camera\n"); return 1;}
cv::namedWindow("image0", cv::WINDOW_AUTOSIZE);
cv::Mat frame0;

for(;;){
  // 探索画像
  cv::Mat search_img = frame0; 
  // テンプレート画像
  cv::Mat tmp_img = cv::imread("circle-1.png", 1);
  if(tmp_img.empty()) return -1; 

  cv::Mat result_img;
  // テンプレートマッチング
  cv::matchTemplate(search_img, tmp_img, result_img, CV_TM_CCOEFF_NORMED);

  // 最大のスコアの場所を探す
  cv::Rect roi_rect(0, 0, tmp_img.cols, tmp_img.rows);
  cv::Point max_pt;
  double maxVal;
  cv::minMaxLoc(result_img, NULL, &maxVal, NULL, &max_pt);
  roi_rect.x = max_pt.x;
  roi_rect.y = max_pt.y;
  std::cout << "(" << max_pt.x << ", " << max_pt.y << "), score=" << maxVal << std::endl;
  // 探索結果の場所に矩形を描画
  cv::rectangle(search_img, roi_rect, cv::Scalar(0,0,255), 3);

  cv::namedWindow("search image", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
  cv::namedWindow("result image", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
  cv::imshow("search image", search_img);
  cv::imshow("result image", result_img);
if(cv::waitKey(30) >= 0) break;
}
}
