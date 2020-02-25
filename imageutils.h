#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <QImage>
#include <opencv4/opencv2/core.hpp>

namespace ImageUtils {
	static cv::Mat QImageToCVMat(const QImage& image);
	static uint64_t AverageHash(const cv::Mat& imgMat);
	static uint64_t BlockMeanHash(const cv::Mat& imgMat);
	static uint64_t PerceptualHash(const cv::Mat& imgMat);
	static uint64_t ColorMomentHash(const cv::Mat& imgMat);
	static uint64_t MarrHildrethHash(const cv::Mat& imgMat);
	static uint64_t RadialVarienceHash(const cv::Mat& imgMat);
}

#endif // IMAGEUTILS_H
