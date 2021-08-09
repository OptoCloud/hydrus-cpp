#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <QImage>
#include <QByteArray>
#include <opencv2/core.hpp>

namespace ImageUtils
{
cv::Size LimitToBounds(const cv::Size& imageRes, const cv::Size& boundingRes);
void ResizeCVMat(const cv::Mat& input, cv::Mat& output, const cv::Size& targetResolution);
std::uint64_t ComputePHash(const cv::Mat& image);
std::uint8_t HammingDistance(std::uint64_t hash1, std::uint64_t hash2);
QByteArray ComputeSha256(const cv::Mat& image);
}

#endif // IMAGEUTILS_H
