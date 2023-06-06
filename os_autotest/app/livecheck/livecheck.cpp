#include "livecheck.h"
using namespace cv;
double Compare_SSIM(Mat image1, Mat image2)
{
    const Scalar C1 = Scalar::all(6.5025), C2 = Scalar::all(58.5225);
    Mat validImage1, validImage2;
    image1.convertTo(validImage1, CV_32F); //数据类型转换为 float,防止后续计算出现错误
    image2.convertTo(validImage2, CV_32F);

    Mat image1_1 = validImage1.mul(validImage1); //图像乘积
    Mat image2_2 = validImage2.mul(validImage2);
    Mat image1_2 = validImage1.mul(validImage2);

    Mat gausBlur1, gausBlur2, gausBlur12;
    GaussianBlur(validImage1, gausBlur1, Size(11, 11), 1.5); //高斯卷积核计算图像均值
    GaussianBlur(validImage2, gausBlur2, Size(11, 11), 1.5);
    GaussianBlur(image1_2, gausBlur12, Size(11, 11), 1.5);

    Mat imageAvgProduct = gausBlur1.mul(gausBlur2); //均值乘积
    Mat u1Squre = gausBlur1.mul(gausBlur1); //各自均值的平方
    Mat u2Squre = gausBlur2.mul(gausBlur2);

    Mat imageConvariance, imageVariance1, imageVariance2;
    Mat squreAvg1, squreAvg2;
    GaussianBlur(image1_1, squreAvg1, Size(11, 11), 1.5); //图像平方的均值
    GaussianBlur(image2_2, squreAvg2, Size(11, 11), 1.5);

    //imageConvariance = gausBlur12 - gausBlur1.mul(gausBlur2);// 计算协方差
    //imageVariance1 = squreAvg1 - gausBlur1.mul(gausBlur1); //计算方差
    //imageVariance2 = squreAvg2 - gausBlur2.mul(gausBlur2);
    absdiff(gausBlur12,gausBlur1.mul(gausBlur2),imageConvariance);
    absdiff(squreAvg1,gausBlur1.mul(gausBlur1),imageVariance1);
    absdiff(squreAvg2,gausBlur2.mul(gausBlur2),imageVariance2);

    auto member = ((2 * gausBlur1 .mul(gausBlur2) + C1).mul(2 * imageConvariance + C2));
    auto denominator = ((u1Squre + u2Squre + C1).mul(imageVariance1 + imageVariance2 + C2));

    Mat ssim;
    divide(member, denominator, ssim);
    //return ssim;

    double sum = 0.0f;
    Scalar meanResult = mean(ssim);
    int channels = ssim.channels();

    for (int depthIndex = 0; depthIndex < channels; ++depthIndex)
    {
        sum += meanResult[depthIndex];
    }
    sum /= channels;

    return sum;
}

double compute_mse(Mat img1,Mat img2){
   Mat diff;
   absdiff(img1,img2,diff);
   diff = diff.mul(diff);
   double err = sum(diff)[0];
   double mse = err/(double(img1.total()));
   return mse;
}

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

int main(int argc, char* argv[])
{

    Mat screeng,screenb;
    Mat screen = capture(1, 1920, 1080);
    //cv::imwrite("screenlive.png", screen, {cv::IMWRITE_PNG_COMPRESSION, 9});
    //cv::cvtColor(screen, screeng, COLOR_BGR2GRAY);
    //threshold(screeng, screenb, 128, 255, THRESH_BINARY);

    cv::Mat ch[9];
    ch[0] = screen(cv::Rect(0,0,640,360));
    ch[1] = screen(cv::Rect(640,0,640,360));
    ch[2] = screen(cv::Rect(1280,0,640,360));
    ch[3] = screen(cv::Rect(0,360,640,360));
    ch[4] = screen(cv::Rect(640,360,640,360));
    ch[5] = screen(cv::Rect(1280,360,640,360));
    ch[6] = screen(cv::Rect(0,720,640,360));
    ch[7] = screen(cv::Rect(640,720,640,360));
    ch[8] = screen(cv::Rect(1280,720,640,360));
    //cv::imwrite("videoloss.png", ch[3], {cv::IMWRITE_PNG_COMPRESSION, 9});
    //return 0;

    Mat videoloss = imread("videoloss.png", IMREAD_UNCHANGED);
    Mat videolossg,videolossb,diff;
    //cv::cvtColor(videoloss, videolossg, COLOR_BGR2GRAY);
    //threshold(videolossg, videolossb, 128, 255, THRESH_BINARY);

    char name[30];
    double ssim[9];
    bool result[9];
    for(int i=0;i<8;++i){
        ssim[i] = Compare_SSIM(ch[i],videoloss);
        fprintf(stderr,"CH%d = %f \n", i+1,ssim[i]);
        result[i] = ssim[i]>0.7 ? 0 : 1;
        printf("CH%d = %d \n", i+1,result[i]);
        // absdiff(ch[i],videolossb,diff);
        // sprintf(name, "diff%d.png", i);
        // cv::imwrite(name, diff, {cv::IMWRITE_PNG_COMPRESSION, 9});
        // mse[i] = compute_mse(ch[i],videolossb);
        // printf("%f ", mse[i]);
        // result[i] = mse[i]>100 ? 1 : 0;
    }

    int bitvalue=0;
    for(int i=0;i<8;++i){
        bitvalue += result[i] << i;
    }

    //const char* text = itoa(bitvalue);
    //printf("%d ", bitvalue);


    return 0;
}
