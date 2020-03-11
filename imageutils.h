#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <QImage>
#include <QByteArray>
#include <opencv/cv.h>

namespace ImageUtils
{
uint64_t PHash_Compute(const cv::Mat& image);
QByteArray Sha256_Compute(const cv::Mat& image);
uint64_t HammingDistance(uint64_t hash1, uint64_t hash2);
}

#endif // IMAGEUTILS_H
