#include "capture.h"

int main(int argc, char* argv[])
{
    int id     = std::stoi(argv[1]);
    int width  = std::stoi(argv[2]);
    int height = std::stoi(argv[3]);

    cv::VideoCapture cap(id);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    if (!cap.isOpened())
        return -1;

    cv::Mat mat{};
    cv::Mat gray{};
    for (int i = 0; i < 100; i++)
    {
        if (!cap.read(mat))
            return -2;

        cv::cvtColor(mat, gray, cv::COLOR_BGR2GRAY);
        if (cv::countNonZero(gray) > 0)
            break;
    }

    if (!cv::imwrite(argv[4], mat, {cv::IMWRITE_PNG_COMPRESSION, 9}))
        return -3;

    return 0;
}
