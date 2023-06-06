#include "ledcheck.h"
using namespace cv;
using namespace std;
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
    printf("%d ",roinum);
    //int diffnum = countNonZero(diff);


    return roinum;
}

Mat getcolormask(Mat src,int color)
{
    Mat srch,bluemask;
    cv::cvtColor(src, srch, COLOR_BGR2HSV);
    Scalar bluelow = Scalar(100,43,46);
    Scalar bluehigh = Scalar(124,255,255);
    Scalar orangelow = Scalar(11,43,46);
    Scalar orangehigh = Scalar(25,255,255);
    Scalar greenlow = Scalar(35,43,46);
    Scalar greenhigh = Scalar(77,255,255);
    if(color==1)
        inRange(srch,bluelow,bluehigh,bluemask);
    else if(color==2)
        inRange(srch,orangelow,orangehigh,bluemask);
    else if(color==3)
        inRange(srch,greenlow,greenhigh,bluemask);
    return bluemask;
}

Mat erode(Mat src){

    Mat dst;
    Mat erode_struct = getStructuringElement(MORPH_RECT,Size(5,5));
    erode(src,dst,erode_struct);
    return dst;
}

Mat dilate(Mat src){

    Mat dst;
    Mat dilate_struct = getStructuringElement(MORPH_RECT,Size(5,5));
    erode(src,dst,dilate_struct);
    return dst;
}

int computecolornum(Mat src,Rect pos){

    Mat roi = src(pos);
    return countNonZero(roi);
}

int sift_search() {

  Mat src = imread("pattern1.png");

  Mat src2 = imread("image_ch1.png");

  //定义Sift的基本参数
  int numFeatures = 500;
  //创建detector存放到KeyPoints中
  Ptr<SIFT> detector = SIFT::create(numFeatures);
  vector<KeyPoint> keypoints, keypoints2;
  detector->detect(src, keypoints);
  detector->detect(src2, keypoints2);
  //打印Keypoints
  cout << "Keypoints:" << keypoints.size() << endl;
  cout << "Keypoints2:" << keypoints2.size() << endl;

  Mat drawsrc, drawsrc2;
  drawKeypoints(src, keypoints, drawsrc);
  //CvUtils::SetShowWindow(drawsrc, "drawsrc", 10, 20);
  //imshow("drawsrc", drawsrc);
  drawKeypoints(src2, keypoints2, drawsrc2);
  //CvUtils::SetShowWindow(drawsrc2, "drawsrc2", 300, 20);
  //imshow("drawsrc2", drawsrc2);

  //计算特征点描述符,特征向量提取
  Mat dstSIFT, dstSIFT2;
  Ptr<SiftDescriptorExtractor> descriptor = SiftDescriptorExtractor::create();
  descriptor->compute(src, keypoints, dstSIFT);
  descriptor->compute(src2, keypoints2, dstSIFT2);
  cout << dstSIFT.cols << endl;
  cout << dstSIFT2.rows << endl;

  //进行BFMatch暴力匹配
  BFMatcher matcher(NORM_L2);
  //定义匹配结果变量
  vector<DMatch> matches;
  //实现描述符之间的匹配
  matcher.match(dstSIFT, dstSIFT2, matches);

  //定义向量距离的最大值与最小值
  double max_dist = 0;
  double min_dist = 1000;
  for (int i = 1; i < dstSIFT.rows; ++i)
  {
    //通过循环更新距离，距离越小越匹配
    double dist = matches[i].distance;
    if (dist > max_dist)
      max_dist = dist;
    if (dist < min_dist)
      min_dist = dist;
  }
  cout << "min_dist=" << min_dist << endl;
  cout << "max_dist=" << max_dist << endl;
  //匹配结果筛选
  vector<DMatch> goodMatches;
  for (int i = 0; i < matches.size(); ++i)
  {
    double dist = matches[i].distance;
    if (dist < 2 * min_dist)
      goodMatches.push_back(matches[i]);
  }
  cout << "goodMatches:" << goodMatches.size() << endl;

  Mat result;
  //匹配特征点天蓝色，单一特征点颜色随机
  drawMatches(src, keypoints, src2, keypoints2, goodMatches, result,
    Scalar(255, 255, 0), Scalar::all(-1));
  //CvUtils::SetShowWindow(result, "Result", 100, 20);
  //imshow("Result", result);
    cv::imwrite("sift_result.png", result, {cv::IMWRITE_PNG_COMPRESSION, 9});

  //waitKey(0);
  return 0;
}

Mat get_ch1_blue(Mat src){

    Rect ch1_rec = Rect(0,0,640,360);
    Rect blue_rec = Rect(150,170,250,160);

    Mat src_ch1 = getcolormask(src(ch1_rec),1);
    //prev_mask_ch1 = getbluemask(prev(ch1_rec));
    Mat element = getStructuringElement(MORPH_RECT,Size(3,3));
    Mat src_ch1_mor;
    morphologyEx(src_ch1, src_ch1_mor,MORPH_OPEN, element);

    Mat blue_roi = src_ch1_mor(blue_rec);
    return blue_roi;
}

Mat get_ch3_green(Mat src){

    Rect rec = Rect(1280,0,640,360);
    Rect color_rec = Rect(150,170,250,160);

    Mat src_rec = getcolormask(src(rec),2);
    //prev_mask_ch1 = getbluemask(prev(ch1_rec));
    Mat element = getStructuringElement(MORPH_RECT,Size(3,3));
    Mat src_mor;
    morphologyEx(src_rec, src_mor,MORPH_OPEN, element);

    Mat roi = src_mor(color_rec);
    return roi;
}

int main(int argc, char* argv[])
{
    // Mat screen1 = capture(1, 1920, 1080);
    // Mat bluemask = getbluemask(screen1);
    // cv::imwrite("bluemask.png", bluemask, {cv::IMWRITE_PNG_COMPRESSION, 9});
    // //sleep(1);
    // // Mat pattern = screen1(Rect(20,130,500,190));
    // // cv::imwrite("pattern1.png", pattern, {cv::IMWRITE_PNG_COMPRESSION, 9});
    // // return 0;

    // Rect ch1_rec = Rect(0,0,640,360);
    // Mat ch1 = screen1(ch1_rec);
    // cv::imwrite("image_ch1.png", ch1, {cv::IMWRITE_PNG_COMPRESSION, 9});
    // Mat ch1_mask_org = bluemask(ch1_rec);
    // int bluenum = countNonZero(ch1_mask_org);
    // cv::imwrite("bluemask_ch1.png", ch1_mask_org, {cv::IMWRITE_PNG_COMPRESSION, 9});

    // //sift_search();
    // Mat element = getStructuringElement(MORPH_RECT,Size(3,3));
    // Mat output;

    // // Closing
    // morphologyEx(ch1_mask_org, output,MORPH_OPEN, element);
    // cv::imwrite("bluemask_ch1_morph.png", output, {cv::IMWRITE_PNG_COMPRESSION, 9});

    // // Mat morb2 = imread("bluemask_ch1_morph_b2.png");
    // // //computecolornum(morb1,Rect(150,170,250,160));
    // // rectangle(morb2,Rect(140,170,330,180),Scalar(0,0,255),5,1,0);
    // // cv::imwrite("bluemask_ch1_morph_b2_rec.png", morb2, {cv::IMWRITE_PNG_COMPRESSION, 9});

    // Mat morg1 = imread("bluemask_ch1_morphg0.png");
    // //computecolornum(morb1,Rect(150,170,250,160));
    // rectangle(morg1,Rect(140,20,330,140),Scalar(0,0,255),5,1,0);
    // cv::imwrite("bluemask_ch1_morphg0_rec.png", morg1, {cv::IMWRITE_PNG_COMPRESSION, 9});

    //return 0;
    //cv::Mat ch1[9];
    //ch1[0] = screen1(cv::Rect(0,0,640,360));
    // ch1[1] = screen1(cv::Rect(640,0,640,360));
    // ch1[2] = screen1(cv::Rect(1280,0,640,360));
    // ch1[3] = screen1(cv::Rect(0,360,640,360));
    // ch1[4] = screen1(cv::Rect(640,360,640,360));
    // ch1[5] = screen1(cv::Rect(1280,360,640,360));
    // ch1[6] = screen1(cv::Rect(0,720,640,360));
    // ch1[7] = screen1(cv::Rect(640,720,640,360));
    // ch1[8] = screen1(cv::Rect(1280,720,640,360));

    #define COLOR b
    #define COLOR_NUM 1 //1:BLUE 2:ORANGE 3:GREEN
    #define CH_NUM 1
    #define ch_name(num) ch_name0(num)
    #define ch_name0(num) ch ## num
    #define color_name(color,num) color_name0(color,num)
    #define color_name0(color,num) color_rec_##color##num

    //delay
    // for(int i=0;i<50000;++i){
    //     int j=0;
    //     ++j;
    // }
    Rect ch1 = Rect(0,0,640,360);
    Rect ch2 = Rect(640,0,640,360);
    Rect ch3 = Rect(1280,0,640,360);
    Rect ch4 = Rect(0,360,640,360);
    Rect ch5 = Rect(640,360,640,360);
    Rect ch6 = Rect(1280,360,640,360);
    Rect ch7 = Rect(0,720,640,360);
    Rect ch8 = Rect(640,720,640,360);
    Rect ch9 = Rect(1280,720,640,360);


    Mat screen = capture(1, 1920, 1080);
    Rect rec = ch_name(CH_NUM);
    Mat ch_img = screen(rec);
    Mat color_img = getcolormask(ch_img,COLOR_NUM);
    Mat element = getStructuringElement(MORPH_RECT,Size(3,3));
    Mat color_morph_gray;
    morphologyEx(color_img, color_morph_gray,MORPH_OPEN, element);
    Mat color_morph;
    cv::cvtColor(color_morph_gray, color_morph, COLOR_GRAY2BGR);
    Rect color_rec_b1 = Rect(150,220,250,110);
    Rect color_rec_g3 = Rect(140,80,300,140);
    Rect color_rec = color_name(COLOR,CH_NUM);
    rectangle(color_morph,color_rec,Scalar(0,0,255),5,1,0);
    cv::imwrite("color_morph.png", color_morph, {cv::IMWRITE_PNG_COMPRESSION, 9});

    rectangle(ch_img,color_rec,Scalar(0,0,255),5,1,0);
    cv::imwrite("color_org.png", ch_img, {cv::IMWRITE_PNG_COMPRESSION, 9});

    return 0;
    Mat first_roi = get_ch1_blue(screen);
    int bluenum = countNonZero(first_roi);

    cv::VideoCapture cap(1);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);

    // if (!cap.isOpened())
    //     return -1;
    Mat prev,diff,now,prev_ch1,diff_mask_ch1,bluemask0,now_mask,prev_mask,now_mask_ch1,prev_mask_ch1,blue_roi,now_mask_ch1_mor;
    Mat mat{};
    Mat gray{};
    int changenum=0,level_blue,threshold_blue=200,switchnum=0,average=bluenum;
    int bluenum_prev=0,bluenum_diff=0,blue_min=90000,blue_max=0;

    level_blue = bluenum > threshold_blue ? 1:0;
    clock_t start, end;
    double time_taken;
    start = clock();

    for(;;)
    {
        cap.read(now);
        cv::cvtColor(now, gray, cv::COLOR_BGR2GRAY);
        if (cv::countNonZero(gray) == 0)
            continue;

        blue_roi = get_ch1_blue(now);
        bluenum = countNonZero(blue_roi);

        if(bluenum == 0)
        {
            break;
        }

        if(bluenum - average > threshold_blue||average - bluenum > threshold_blue)
        {
            average = bluenum;
            ++switchnum;
        }
        else
        {
            average = (average + bluenum)/2;
        }

        fprintf(stderr,"ch1 blue num : %d ,",bluenum);
        fprintf(stderr,"average : %d ,",average);
        fprintf(stderr,"switchnum : %d\n",switchnum);

        end = clock();
        time_taken = double(end - start) / double(CLOCKS_PER_SEC);

        fprintf(stderr,"time_taken : %f sec \n",time_taken);
        if(time_taken >= 6)
            break;

    }
    int on = switchnum>=2 ? 1 : 0;

    printf("%d ", on);

    return 0;

    Mat screen2 = capture(1, 1920, 1080);
    // cv::imwrite("screenreplay2.png", screen2, {cv::IMWRITE_PNG_COMPRESSION, 9});
    // cv::cvtColor(screen2, screen2g, cv::COLOR_BGR2GRAY);
    // threshold(screen2g, screen2b, 128, 255, THRESH_BINARY);
    //absdiff(screen1b,screen2b,screendiff);
    //int screennum = countNonZero(screendiff);
    //printf("%d \n",screennum);
    //cv::imwrite("screenbdiff.png", screendiff, {cv::IMWRITE_PNG_COMPRESSION, 9});

    // cv::Mat ch2[9];
    // ch2[0] = screen2b(cv::Rect(0,0,640,360));
    // ch2[1] = screen2b(cv::Rect(640,0,640,360));
    // ch2[2] = screen2b(cv::Rect(1280,0,640,360));
    // ch2[3] = screen2b(cv::Rect(0,360,640,360));
    // ch2[4] = screen2b(cv::Rect(640,360,640,360));
    // ch2[5] = screen2b(cv::Rect(1280,360,640,360));
    // ch2[6] = screen2b(cv::Rect(0,720,640,360));
    // ch2[7] = screen2b(cv::Rect(640,720,640,360));
    // ch2[8] = screen2b(cv::Rect(1280,720,640,360));

    //Mat chg1[9],chg2[9],chb1[9],chb2[9];
    Mat chdiff[9];
    bool result[9];

    for(int i=0;i<9;i++){
        // cv::cvtColor(ch1[i], chg1[i], cv::COLOR_BGR2GRAY);
        // threshold(chg1[i], chb1[i], 128, 255, THRESH_BINARY);
        // cv::cvtColor(ch2[i], chg2[i], cv::COLOR_BGR2GRAY);
        // threshold(chg2[i], chb2[i], 128, 255, THRESH_BINARY);
        //absdiff(ch1[i],ch2[i],chdiff[i]);
        result[i] = (bool)checktimestamp(chdiff[i]);
    }

    int bitvalue=0;

    for(int i=0;i<8;++i){
        bitvalue += result[i] << i;
    }

    //const char* text = itoa(bitvalue);
    printf("%d ", bitvalue);

    return 0;
    //return 72;
}
