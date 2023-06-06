#include "crop.h"

int main(int argc, char* argv[])
{
    int x0 = std::stoi(argv[2]);
    int y0 = std::stoi(argv[3]);
    int x1 = std::stoi(argv[4]);
    int y1 = std::stoi(argv[5]);

    cv::Mat mat = cv::imread(argv[1]);

    cv::Mat cropped = mat(cv::Rect(x0, y0, x1 - x0, y1 - y0));
    cv::imwrite(argv[6], cropped, {cv::IMWRITE_PNG_COMPRESSION, 9});

    return 0;
}
