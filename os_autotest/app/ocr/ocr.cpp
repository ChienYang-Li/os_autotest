#include "ocr.h"

int main(int argc, char* argv[])
{
    cv::Mat mat = cv::imread(argv[1]);

    cv::Mat gray{};
    cv::cvtColor(mat, gray, cv::COLOR_RGB2GRAY);

    cv::Mat img{};
    cv::bitwise_not(gray, img);

    tesseract::TessBaseAPI tess{};
    tess.Init(NULL, "eng", tesseract::OEM_DEFAULT);
    tess.SetPageSegMode(tesseract::PSM_SINGLE_LINE);
    tess.SetImage((uchar*)img.data, img.cols, img.rows, 1, img.step);

    const char* text = tess.GetUTF8Text();
    printf("%s", text);

    return 0;
}
