#include "imageutils.h"

#include <QFile>
#include <QDebug>
#include <bitset>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/img_hash.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/highgui.hpp>

uint64_t ImageUtils::PHash_Compute(const QImage& qtImage)
{
	if (qtImage.isNull())
		return 0;

	cv::Mat hash;
	cv::Mat cvImage;

	if (qtImage.format() == QImage::Format_RGB888)
	{
		cvImage = cv::Mat(qtImage.height(), qtImage.width(), CV_8UC3, (uint8_t*)qtImage.constBits(), qtImage.bytesPerLine());

		if (cvImage.empty())
			return 0;

		cv::img_hash::pHash(cvImage, hash);
	}
	else
	{
		QImage rgb888 = qtImage.convertToFormat(QImage::Format_RGB888);
		cvImage = cv::Mat(rgb888.height(), rgb888.width(), CV_8UC3, (uint8_t*)rgb888.constBits(), rgb888.bytesPerLine());

		if (cvImage.empty())
			return 0;

		cv::img_hash::pHash(cvImage, hash);
	}

	if (hash.rows * hash.cols <= 0)
		return 0;

	uint64_t retval;
	memcpy(&retval, hash.data, hash.rows*hash.cols);
	return retval;
}
uint16_t ImageUtils::HammingDistance(uint64_t hash1, uint64_t hash2)
{
	uint64_t diff = hash1 ^ hash2;

	std::bitset<64> diff_bits(diff);

	return diff_bits.count();
}

#include "sha256.h"
#include <QByteArray>
QByteArray ImageUtils::Sha256_Compute(const QImage& image)
{
	Hashing::Sha256 hash;
	hash.Update(image.constBits(), image.byteCount());
	return hash.result();
}
bool DecodeImageTest(const QString& path)
{
	cv::Mat img = cv::imread(path.toStdString());

	cv::imshow("eee", img);
}
bool Test(const QImage& inputImage, uint64_t& hash)
{
	// Return if image is empty
	if (inputImage.isNull())
		return false;

	cv::Mat grayScale;

	/*QImage image;
	if (inputImage.height() > 1024 || inputImage.width() > 1024)
		image = inputImage.scaled(1024, 1024, Qt::KeepAspectRatio);
	else
		image = inputImage;*/


	// Convert to uint8 pic
	/*switch (inputImage.format()) {
	case QImage::Format_Grayscale8:
		break;
	case QImage::Format_RGB444:
	case QImage::Format_RGB555:
	case QImage::Format_RGB666:
	case QImage::Format_RGB888:
	case QImage::Format_RGB16:
	case QImage::Format_RGB30:
	case QImage::Format_RGB32:
		image = image.convertToFormat(QImage::Format_Grayscale8);
		grayScale = cv::Mat(image.height(), image.width(), CV_8UC1, (uint8_t*)image.constBits(), image.bytesPerLine());
		break;
	case QImage::Format_ARGB32:
		image = image.convertToFormat(QImage::Format_RGBA8888);
	case QImage::Format_RGBA8888:
	{
		uint8_t* data = image.bits();
		uint32_t size = image.byteCount();

		QByteArray arr((char*)data, size);
		qDebug() << arr.toHex();

		grayScale = cv::Mat(image.height(), image.width(), CV_8UC1, (uint8_t*)image.constBits(), image.bytesPerLine());
		break;
	}
	default:
		// TODO
		break;
	}

	uint8_t* data = image.bits();
	uint32_t size = image.byteCount();

	// Convert to greyscale
	switch (inputImage.format()) {
	case QImage::Format_RGB444:
	case QImage::Format_RGB555:
	case QImage::Format_RGB666:
	case QImage::Format_RGB888:
	case QImage::Format_RGB16:
	case QImage::Format_RGB30:
	case QImage::Format_RGB32:
	{
		image = image.convertToFormat(QImage::Format_Grayscale8);
		data = image.bits();
	}
	case QImage::Format_Grayscale8:
	{
		grayScale = cv::Mat(image.height(), image.width(), CV_8UC1, (uint8_t*)image.constBits(), image.bytesPerLine());
		break;
	}
	case QImage::Format_ARGB32:
	{
		image = inputImage.convertToFormat(QImage::Format_RGBA8888);
		image.bits();
	}
	case QImage::Format_RGBA8888:
	{
		cv::Mat tmp = cv::Mat(image.height(), image.width(), CV_8UC4, (uint8_t*)image.constBits(), image.bytesPerLine());

		grayScale = cv::Mat(image.height(), image.width(), CV_8UC1, (uint8_t*)image.constBits(), image.bytesPerLine());
		break;
	}
	default:
		// TODO
		break;
	}

	if (grayScale.empty())
		return false;

	cv::Mat hashMat;
	cv::img_hash::pHash(grayScale, hashMat);

	memcpy(&hash, hashMat.data, hashMat.rows*hashMat.cols);
	return true;*/
}
