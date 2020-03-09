#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <QImage>
#include <QPixmap>
#include <opencv4/opencv2/core.hpp>

namespace ImageUtils
{
struct HASH256
{
	uint8_t* data;
};

uint64_t PHash_Compute(const QImage& image);
uint16_t HammingDistance(uint64_t hash1, uint64_t hash2);
QByteArray Sha256_Compute(const QImage& image);
}

bool DecodeImageTest(const QString& path);
bool Test(const QImage& inputImage, uint64_t& hash);

#endif // IMAGEUTILS_H
