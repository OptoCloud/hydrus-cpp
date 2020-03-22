#include "imageutils.h"

#include <QFile>
#include <QDebug>
#include <bitset>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

uint64_t ImageUtils::PHash_Compute(const cv::Mat& inputImage)
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

    // Convert to uint64
    uint8_t* ptr = dct_bool.ptr<uint8_t>(0);
    uint64_t hash = 0;
    for (uint64_t i = 64; i > 0;)
    {
        hash |= (*ptr++ & 1) << --i;
    }

	return hash;
}
uint64_t ImageUtils::HammingDistance(uint64_t hash1, uint64_t hash2)
{
    return std::bitset<64>(hash1 ^ hash2).count();
}

#include "sha256.h"
#include <QByteArray>
QByteArray ImageUtils::Sha256_Compute(const cv::Mat& image)
{
	Hashing::Sha256 hash;
	hash.Update(image.data, image.total() * image.elemSize());
	return hash.result();
}
