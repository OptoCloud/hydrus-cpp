#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <QImage>
#include <QByteArray>

namespace ImageUtils
{
uint64_t PHash_Compute(const QByteArray& data);
uint16_t HammingDistance(uint64_t hash1, uint64_t hash2);
QByteArray Sha256_Compute(const QImage& image);
}

#endif // IMAGEUTILS_H
