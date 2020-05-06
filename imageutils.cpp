#include "imageutils.h"

#include <QFile>
#include <QDebug>
#include <bitset>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

quint64 ImageUtils::ComputePHash(const cv::Mat& inputImage)
{
	cv::Mat image;
	inputImage.copyTo(image);

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

        // Convert to grayscale
		cv::Mat grayscale;
        cv::cvtColor(image, grayscale, cv::COLOR_BGR2GRAY);
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
        cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
		image.convertTo(image, CV_32F);
		break;
	case 1:
		break;
	default:
		return 0;
	}

	// Crunch it down
	cv::resize(image,image,cv::Size(32,32), cv::INTER_AREA);

	// Convert to dct
	cv::dct(image, image);

	// Get region of intrest
	image(cv::Rect(0,0,8,8)).copyTo(image);

	// Get mean color of roi
	cv::Scalar meanColor = cv::mean(image.reshape(1,1)(cv::Rect(1,0,63,1)));
	cv::Mat meanMat = cv::Mat(8,8,CV_32F,meanColor);

	// Convert roi to bool map
	cv::Mat dct_bool = image > meanMat;

	// Array of bytes that are either 0x00 or 0xFF
	quint8* ptr = dct_bool.ptr<quint8>(0);

	quint64 hash = 0;
	// Iterates trough the 64 bytes and masks every byte to a bit in the 64 bit integer
	for (quint64 i = 64; i > 0; i--)
    {
		// Mask byte to a bit
		quint64 masked = *ptr & 1;

		// Shift bit into its position
		hash |= masked << i;

		// Increment by one byte
		ptr++;
    }

	return hash;
}
quint64 ImageUtils::HammingDistance(quint64 hash1, quint64 hash2)
{
	// Get which bits are different
	quint64 diff = hash1 ^ hash2;

	// Count the b
	return std::bitset<64>(diff).count();
}

#include "sha256.h"
#include <QByteArray>
QByteArray ImageUtils::ComputeSha256(const cv::Mat& image)
{
	Hashing::Sha256 hash;
	hash.update(image.data, image.total() * image.elemSize());
	return hash.result();
}
