#include "filter.h"

int main(int argc, char* argv[])
{
    //int x0 = std::stoi(argv[2]);
    //int y0 = std::stoi(argv[3]);
    //int x1 = std::stoi(argv[4]);
    //int y1 = std::stoi(argv[5]);

    cv::Mat src = cv::imread(argv[1]);
    cv::Mat src_gray;
    cvtColor( src, src_gray, cv::COLOR_BGR2GRAY );
    int thresh = 100;
    cv::Mat canny_output;
    Canny( src_gray, canny_output, 150, 200 );
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    findContours( canny_output, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE );
    cv::Mat drawing = cv::Mat::zeros( canny_output.size(), CV_8UC3 );
    cv::Scalar color = cv::Scalar( 210,130,210 );
    for (unsigned int i = 0;  i < contours.size();  i++)
    {
        std::cout << "# of contour points: " << contours[i].size() << std::endl;

        // for (unsigned int j=0;  j<contours[i].size();  j++)
        // {
        //     std::cout << "Point(x,y)=" << contours[i][j] << std::endl;
        // }

        //std::cout << " Area: " << contourArea(contours[i]) << std::endl;

        if(contours[i].size()>10){
            drawContours( drawing, contours, (int)i, color, 2, cv::LINE_8, hierarchy, 0 );
        }
    }

    // for( size_t i = 0; i< contours.size(); i++ )
    // {
    //     cv::Scalar color = cv::Scalar( 210,130,210 );
    //     drawContours( drawing, contours, (int)i, color, 2, cv::LINE_8, hierarchy, 0 );
    // }
    //imshow( "Contours", drawing );

    //cv::Mat filtered = mat(cv::Rect(x0, y0, x1 - x0, y1 - y0));
    cv::imwrite(argv[2], drawing, {cv::IMWRITE_PNG_COMPRESSION, 9});

    return 0;
}
