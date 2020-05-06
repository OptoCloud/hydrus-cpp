#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <QImage>
#include <QByteArray>
#include <opencv2/core.hpp>

namespace ImageUtils
{
quint64 ComputePHash(const cv::Mat& image);
QByteArray ComputeSha256(const cv::Mat& image);
quint64 HammingDistance(quint64 hash1, quint64 hash2);
}

#endif // IMAGEUTILS_H
