#include "timestampcheck.h"
using namespace cv;

Mat capture(int id ,int width ,int height)
{

    cv::VideoCapture cap(id);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    // if (!cap.isOpened())
    //     return -1;

    cv::Mat mat{};
    cv::Mat gray{};
    for (int i = 0; i < 100; i++)
    {
        cap.read(mat);
        cv::cvtColor(mat, gray, cv::COLOR_BGR2GRAY);
        if (cv::countNonZero(gray) > 0)
            break;
    }

    //cv::imwrite(name, mat, {cv::IMWRITE_PNG_COMPRESSION, 9};

    return mat;
}

int checktimestamp(Mat diff){

    Mat roi = diff(Rect(0,0,200,40));

    int roinum = countNonZero(roi);
    //printf("%d ",roinum);
    //int diffnum = countNonZero(diff);


    return roinum;
}

int main(int argc, char* argv[])
{
    Mat screen1g,screen2g,screen1b,screen2b,screendiff;
    Mat screen1 = capture(1, 1920, 1080);
    //cv::imwrite("screenreplay1.png", screen1, {cv::IMWRITE_PNG_COMPRESSION, 9});
    cv::cvtColor(screen1, screen1g, COLOR_BGR2GRAY);
    threshold(screen1g, screen1b, 128, 255, THRESH_BINARY);

    cv::Mat ch1[9];
    ch1[0] = screen1b(cv::Rect(0,0,640,360));
    ch1[1] = screen1b(cv::Rect(640,0,640,360));
    ch1[2] = screen1b(cv::Rect(1280,0,640,360));
    ch1[3] = screen1b(cv::Rect(0,360,640,360));
    ch1[4] = screen1b(cv::Rect(640,360,640,360));
    ch1[5] = screen1b(cv::Rect(1280,360,640,360));
    ch1[6] = screen1b(cv::Rect(0,720,640,360));
    ch1[7] = screen1b(cv::Rect(640,720,640,360));
    ch1[8] = screen1b(cv::Rect(1280,720,640,360));


    //delay
    for(int i=0;i<50000;++i){
        int j=0;
        ++j;
    }

    Mat screen2 = capture(1, 1920, 1080);
    //cv::imwrite("screenreplay2.png", screen2, {cv::IMWRITE_PNG_COMPRESSION, 9});
    cv::cvtColor(screen2, screen2g, cv::COLOR_BGR2GRAY);
    threshold(screen2g, screen2b, 128, 255, THRESH_BINARY);
    //absdiff(screen1b,screen2b,screendiff);
    //int screennum = countNonZero(screendiff);
    //printf("%d \n",screennum);
    //cv::imwrite("screenbdiff.png", screendiff, {cv::IMWRITE_PNG_COMPRESSION, 9});

    cv::Mat ch2[9];
    ch2[0] = screen2b(cv::Rect(0,0,640,360));
    ch2[1] = screen2b(cv::Rect(640,0,640,360));
    ch2[2] = screen2b(cv::Rect(1280,0,640,360));
    ch2[3] = screen2b(cv::Rect(0,360,640,360));
    ch2[4] = screen2b(cv::Rect(640,360,640,360));
    ch2[5] = screen2b(cv::Rect(1280,360,640,360));
    ch2[6] = screen2b(cv::Rect(0,720,640,360));
    ch2[7] = screen2b(cv::Rect(640,720,640,360));
    ch2[8] = screen2b(cv::Rect(1280,720,640,360));

    //Mat chg1[9],chg2[9],chb1[9],chb2[9];
    Mat chdiff[9];
    bool result[9];

    for(int i=0;i<8;i++){
        // cv::cvtColor(ch1[i], chg1[i], cv::COLOR_BGR2GRAY);
        // threshold(chg1[i], chb1[i], 128, 255, THRESH_BINARY);
        // cv::cvtColor(ch2[i], chg2[i], cv::COLOR_BGR2GRAY);
        // threshold(chg2[i], chb2[i], 128, 255, THRESH_BINARY);
        absdiff(ch1[i],ch2[i],chdiff[i]);
        result[i] = (bool)checktimestamp(chdiff[i]);
        printf("CH%d = %d\n",i+1,result[i]);
    }

    int bitvalue=0;

    for(int i=0;i<8;++i){
        bitvalue += result[i] << i;
    }

    //const char* text = itoa(bitvalue);
    //printf("%d ", bitvalue);

    return 0;
    //return 72;
}







