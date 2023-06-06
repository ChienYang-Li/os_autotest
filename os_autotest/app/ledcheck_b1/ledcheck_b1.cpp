#include "ledcheck_b1.h"
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

Mat get_ch1_blue(Mat src,Rect color_rec){

    Rect ch_rec = Rect(0,0,640,360);

    Mat src_ch1 = getcolormask(src(ch_rec),1);
    //prev_mask_ch1 = getbluemask(prev(ch1_rec));
    Mat element = getStructuringElement(MORPH_RECT,Size(3,3));
    Mat src_ch1_mor;
    morphologyEx(src_ch1, src_ch1_mor,MORPH_OPEN, element);

    Mat blue_roi = src_ch1_mor(color_rec);
    return blue_roi;
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


    //delay
    // for(int i=0;i<50000;++i){
    //     int j=0;
    //     ++j;
    // }
    Mat screen = capture(1, 1920, 1080);
    Rect color_rec = Rect(90,190,430,150);
    Mat first_roi = get_ch1_blue(screen,color_rec);
    int colornum = countNonZero(first_roi);

    cv::VideoCapture cap(1);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);

    // if (!cap.isOpened())
    //     return -1;
    Mat prev,diff,now,prev_ch1,diff_mask_ch1,bluemask0,now_mask,prev_mask,now_mask_ch1,prev_mask_ch1,roi,now_mask_ch1_mor;
    Mat mat{};
    Mat gray{};
    int changenum=0,level,threshold=200,switchnum=0,average=colornum,count=0;
    int bluenum_prev=0,bluenum_diff=0,blue_min=90000,blue_max=0;

    //level = colornum > threshold ? 1:0;
    clock_t start, end;
    double time_taken;
    start = clock();

    for(;;)
    {
        cap.read(now);
        cv::cvtColor(now, gray, cv::COLOR_BGR2GRAY);
        if (cv::countNonZero(gray) == 0)
            continue;

        roi = get_ch1_blue(now,color_rec);
        colornum = countNonZero(roi);

        // if(colornum == 0)
        // {
        //     break;
        // }

        if(colornum - average > threshold||average - colornum > threshold)
        {
            ++count;
            if(count>=4){
                count = 0;
                average = colornum;
                ++switchnum;
            }
        }
        else
        {
            average = (average + colornum)/2;
            count = 0;
        }

        fprintf(stderr,"color num : %d ,",colornum);
        fprintf(stderr,"average : %d ,",average);
        fprintf(stderr,"switchnum : %d\n",switchnum);

        end = clock();
        time_taken = double(end - start) / double(CLOCKS_PER_SEC);

        fprintf(stderr,"time_taken : %f sec \n",time_taken);
        if(time_taken >= 6)
            break;

    }

    Rect rec = Rect(0,0,640,360);
    Rect led_pos = Rect(210,270,190,40);
    Mat ch_img = now(rec);
    //cv::imwrite("ledcheck_b1.png", ch_img, {cv::IMWRITE_PNG_COMPRESSION, 9});
    Mat color_img = getcolormask(ch_img,1);
    Mat element = getStructuringElement(MORPH_RECT,Size(3,3));
    Mat color_morph_gray;
    morphologyEx(color_img, color_morph_gray,MORPH_OPEN, element);
    Mat color_morph;
    cv::cvtColor(color_morph_gray, color_morph, COLOR_GRAY2BGR);
    rectangle(color_morph,color_rec,Scalar(0,0,255),5,1,0);
    //cv::imwrite("ledcheck_b1_mask.png", color_morph, {cv::IMWRITE_PNG_COMPRESSION, 9});

    Mat ch_img2 = ch_img;
    rectangle(ch_img,led_pos,Scalar(0,0,255),5,1,0);
    //cv::imwrite("ledcheck_b1_rect.png", ch_img, {cv::IMWRITE_PNG_COMPRESSION, 9});
    rectangle(ch_img2,color_rec,Scalar(0,0,255),5,1,0);
    //cv::imwrite("ledcheck_b1_rect2.png", ch_img, {cv::IMWRITE_PNG_COMPRESSION, 9});

    int replay_on = switchnum>=2 ? 1 : 0;

    int threshold_power = 500,power_on=0;
    if(colornum>threshold_power){
        power_on = 1;
    }

    printf("%d %d", replay_on,power_on);

    return 0;
}
