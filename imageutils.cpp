#include "imageutils.h"

#include <opencv4/opencv2/img_hash.hpp>

/*bool OpenCV_pHash(const QImage& src, uint64_t& hash)
{
	if (src.isNull())
		return false;

	try {
		// Do work here
	} catch (cv::Exception ex) {
		qDebug() << "OpenCV_pHash error:" << ex.msg.c_str();
	} catch (std::exception ex) {
		qDebug() << "OpenCV_pHash error:" << ex.what();
	} catch (int ex) {
		qDebug() << "OpenCV_pHash error:" << "Error:" << ex;
	}
	return false;
}*/

cv::Mat ImageUtils::QImageToCVMat(const QImage& qimage)
{
	if (qimage.format() == QImage::Format_RGB888) {
		return cv::Mat(qimage.height(), qimage.width(), CV_8UC3, (uint8_t*)qimage.constBits(), qimage.bytesPerLine());
	} else {
		QImage rgb888 = qimage.convertToFormat(QImage::Format_RGB888);
		return cv::Mat(rgb888.height(), rgb888.width(), CV_8UC3, (uint8_t*)rgb888.constBits(), rgb888.bytesPerLine());
	}
}

uint64_t ImageUtils::PerceptualHash(const cv::Mat& imgMat)
{
	// Perform hashing
	cv::Mat hash;
	cv::img_hash::pHash(imgMat, hash);

	// If hash is zero
	if (hash.rows * hash.cols <= 0)
		return 0;

	uint64_t retval;
	memcpy(&retval, hash.data, hash.rows*hash.cols);
	return retval;
}

uint64_t ImageUtils::AverageHash(const cv::Mat& imgMat)
{
	// Perform hashing
	cv::Mat hash;
	cv::img_hash::averageHash(imgMat, hash);

	// If hash is zero
	if (hash.rows * hash.cols <= 0)
		return 0;

	uint64_t retval;
	memcpy(&retval, hash.data, hash.rows*hash.cols);
	return retval;
}

uint64_t ImageUtils::BlockMeanHash(const cv::Mat& imgMat)
{
	// Perform hashing
	cv::Mat hash;
	cv::img_hash::blockMeanHash(imgMat, hash);

	// If hash is zero
	if (hash.rows * hash.cols <= 0)
		return 0;

	uint64_t retval;
	memcpy(&retval, hash.data, hash.rows*hash.cols);
	return retval;
}

uint64_t ImageUtils::ColorMomentHash(const cv::Mat& imgMat)
{
	// Perform hashing
	cv::Mat hash;
	cv::img_hash::colorMomentHash(imgMat, hash);

	// If hash is zero
	if (hash.rows * hash.cols <= 0)
		return 0;

	uint64_t retval;
	memcpy(&retval, hash.data, hash.rows*hash.cols);
	return retval;
}

uint64_t ImageUtils::MarrHildrethHash(const cv::Mat& imgMat)
{
	// Perform hashing
	cv::Mat hash;
	cv::img_hash::marrHildrethHash(imgMat, hash);

	// If hash is zero
	if (hash.rows * hash.cols <= 0)
		return 0;

	uint64_t retval;
	memcpy(&retval, hash.data, hash.rows*hash.cols);
	return retval;
}

uint64_t ImageUtils::RadialVarienceHash(const cv::Mat& imgMat)
{
	// Perform hashing
	cv::Mat hash;
	cv::img_hash::radialVarianceHash(imgMat, hash);

	// If hash is zero
	if (hash.rows * hash.cols <= 0)
		return 0;

	uint64_t retval;
	memcpy(&retval, hash.data, hash.rows*hash.cols);
	return retval;
}
