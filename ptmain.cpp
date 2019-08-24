#include "Particle.hpp"

using namespace std;

int main(void){

    cv::VideoCapture video;
    video.open(1);

    if (!video.isOpened()){
        cout << "can't open your video" << endl;
    }

    cv::namedWindow("Camera", CV_WINDOW_NORMAL);

    bool start = false;

    ParticleFilter *pf = new ParticleFilter();
    while (1){
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
        pf->CalcWeight(frame);

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


        cv::imshow("Camera", frame);

        char c = (char)cv::waitKey(1);

        if (c == 'q')
            break;
    }

}