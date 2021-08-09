#include "imageutils.h"

#include <QFile>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include <iostream>
#include <bitset>

cv::Size ImageUtils::LimitToBounds(const cv::Size& imageRes, const cv::Size& boundingRes)
{
    if (boundingRes.width >= imageRes.width && boundingRes.height >= imageRes.height)
    {
        return imageRes;
    }

    cv::Size thumbnailRes = boundingRes;

    if (imageRes.width > imageRes.height) {
        int height = imageRes.height * ((float)boundingRes.width / (float)imageRes.width);

        thumbnailRes.height = std::max(height, 1);
        thumbnailRes.width = std::max(thumbnailRes.width, 1);
    }
    else if (imageRes.height > imageRes.width) {
        int width = imageRes.width * ((float)boundingRes.height / (float)imageRes.height);

        thumbnailRes.width = std::max(width, 1);
        thumbnailRes.height = std::max(thumbnailRes.height, 1);
    }

    return thumbnailRes;
}

void ImageUtils::ResizeCVMat(const cv::Mat& input, cv::Mat& output, const cv::Size& targetResolution)
{
    cv::Size imageRes(input.cols, input.rows);

    if (targetResolution == imageRes) {
        output = input.clone();
    }

    cv::InterpolationFlags interpolation;
    if (targetResolution.width > imageRes.width || targetResolution.height > imageRes.height) {
        interpolation = cv::INTER_LANCZOS4;
    } else {
        interpolation = cv::INTER_AREA;
    }

    cv::resize(input, output, targetResolution, interpolation);
}

std::uint64_t ImageUtils::ComputePHash(const cv::Mat& image)
{
    // Convert to grayscale
    cv::Mat grayscale;
    cv::cvtColor(image, grayscale, cv::COLOR_RGB2GRAY);

    // If we have a alpha channel as well, we gotta do some stuff to apply that to the grayscale result
    if (image.channels() == 4) {
        // Get alpha
        cv::Mat alpha;
        cv::extractChannel(image, alpha, 3);

        // Get alpha normalized (0.0 - 1.0)
        cv::Mat alpha_normalized = alpha / 255.f;

        // Get inverse of alpha (0.0 - 255.0)
        cv::Mat alpha_inverted = 255.f - alpha;

        // Blend alpha with grayscale
        cv::multiply(grayscale, alpha_normalized, grayscale);

        // Image is a result of the inverted alpha plus the greyscale
        cv::add(grayscale, alpha_inverted, grayscale);
    }

	// Crunch it down
    cv::Mat tiny;
    cv::resize(grayscale, tiny, cv::Size(32,32), cv::INTER_AREA);

    // DCT cant be done with u8 matrix, Weird!
    tiny.convertTo(tiny, CV_32FC1);

	// Convert to dct
    cv::Mat dct;
    cv::dct(tiny, dct);
    dct.convertTo(dct, CV_8UC1);

	// Get region of intrest
    cv::Mat dct_88 = dct(cv::Rect(0,0,8,8));
    cv::imwrite("dct_88.png", dct_88);

    // Get mean color of matrix (except [0,0] which we will exclude because it represents flat colour)
    std::uint8_t mean = 0;
    for (int y = 0; y < 8; y++) {
        for (int x = 1; x < 8; x++) {
            int idx = (y * dct_88.step) + x;

            mean += dct_88.data[idx];
        }
    }
    mean /= 63; // Average out the accumulated numbers

    // Set bits compared to their index in the 8x8 matrix
    std::uint64_t phash = 0;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            int idx = (y * dct_88.step) + x;

            // If the pixel is more than the average, set the value to 1, else 0
            std::uint64_t isMoreThanAverage = dct_88.data[idx] > mean;

            // Shift the bit into the correct place for this index
            isMoreThanAverage <<= (y * 8) + x;

            // Set the bit in the hash
            phash |= isMoreThanAverage;
        }
    }

    std::cout << std::hex << phash << std::dec << std::endl;

    return phash;
}

std::uint8_t ImageUtils::HammingDistance(std::uint64_t hash1, std::uint64_t hash2)
{
	// Get which bits are different
    std::uint64_t diff = hash1 ^ hash2;

	// Count bits which are different
	return std::bitset<64>(diff).count();
}

#include <QByteArray>
QByteArray ImageUtils::ComputeSha256(const cv::Mat& image)
{
	//Hashing::Sha256 hash;
	//hash.update(image.data, image.total() * image.elemSize());
	return QByteArray();//hash.result();
}
