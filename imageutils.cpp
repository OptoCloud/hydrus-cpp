#include "imageutils.h"

#include <QFile>
#include <QDebug>
#include <bitset>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

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

	}
	else
	{
		QImage rgb888 = qtImage.convertToFormat(QImage::Format_RGB888);
		cvImage = cv::Mat(rgb888.height(), rgb888.width(), CV_8UC3, (uint8_t*)rgb888.constBits(), rgb888.bytesPerLine());

		if (cvImage.empty())
			return 0;

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
bool PHashCust(const QByteArray& data, uint64_t& hash)
{
	auto dataMat = cv::Mat(data.size(), 1, CV_8UC1, (char*)data.data());
	auto image = cv::imdecode(dataMat, cv::IMREAD_UNCHANGED);

	if (image.empty())
		return false;

	// Remove alpha if neccesary
	switch (image.channels()) {
	case 4:
	{
		// Resize to conserve ram
		int maxDim = std::max(image.rows, image.cols);
		if (maxDim > 1024)
		{
			float ratio = float(1024)/float(maxDim);

			cv::resize(image, image, cv::Size(0, 0), ratio, ratio);
		}
		cv::Size size(image.cols, image.rows);

		// Get alpha
		cv::Mat alpha;
		cv::extractChannel(image, alpha, 3);
		alpha.convertTo(alpha, CV_32F);
		alpha /= 255.;

		// Convert to grayscale (RGB was done intentionally)
		cv::Mat grayscale;
		cv::cvtColor(image, grayscale, cv::COLOR_RGB2GRAY);
		grayscale.convertTo(grayscale, CV_32FC1);

		// Create white canvas
		cv::Mat ones = cv::Mat::ones(size, CV_32FC1);

		// Blend alpha with white, and set as image
		cv::multiply(ones * 255., ones - alpha, image);

		// Blend alpha with grayscale and add to output
		cv::multiply(grayscale,alpha,grayscale);
		image += grayscale;
		break;
	}
	case 3:
		cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
		image.convertTo(image, CV_32F);
		break;
	case 1:
		break;
	default:
		return false;
	}

	// Crunch it down
	cv::resize(image,image,cv::Size(32,32), cv::INTER_AREA);

	// Convert to dct
	cv::dct(image, image);

	// Get region of intrest
	cv::Mat roi = image(cv::Rect(0,0,8,8));
	roi.copyTo(image);

	// Get mean color of roi
	cv::Scalar meanColor = cv::mean(image.reshape(1,1)(cv::Rect(1,0,63,1)));
	cv::Mat meanMat = cv::Mat(8,8,CV_32F,meanColor);

	// Convert roi to bool map
	cv::Mat dct_bool = image > meanMat;

	// Convert to uint64
	QByteArray byteHash((char*)dct_bool.data, 64);
	QString hex = byteHash.toHex();
	std::bitset<64> hashBits;
	for (int i = 0; i < 64; ++i)
	{
		if (hex[i] == 'f')
			hashBits[i] = true;
		else
			hashBits[i] = false;
	}
	memcpy(&hash, &hashBits, 8);
}
