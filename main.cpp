#include "mainwindow.h"

#include <QApplication>
#include <QPushButton>

#include <QGraphicsView>
#include <QGraphicsScene>
#include "hydrusthumbnailitem.h"

#include <QDir>
#include <QDebug>
#include <clientdb.h>

#ifdef __linux__
#include <opencv2/core.hpp>
#include <opencv4/opencv2/img_hash/phash.hpp>
#elif _WIN32
#else
#error "Unsupported OS!"
#endif

#define IMAGE_DIR "../" // "D:\\MEDIA LIBRARY\\nature\\"

bool OpenCV_pHash(const QImage& src, uint64_t& hash)
{	
	if (src.isNull())
		return false;

	try {
		QImage rgb888;

		// Make copy of image (Unkown if nessecary)
		if (src.format() == QImage::Format_RGB888) {
			rgb888 = src;
		} else {
			rgb888 = src.convertToFormat(QImage::Format_RGB888);
		}

		// Convert QImage to OpenCV::Mat
		cv::Mat imageMat(rgb888.height(), rgb888.width(), CV_8UC3, (uint8_t*)rgb888.constBits(), rgb888.bytesPerLine());

		// Perform hashing
		cv::Mat hashMatrix;
		cv::img_hash::pHash(imageMat, hashMatrix);

		// If hash is zero
		if (hashMatrix.rows * hashMatrix.cols <= 0)
			return false;

		memcpy(&hash, hashMatrix.data, hashMatrix.rows*hashMatrix.cols);
		return true;
	} catch (cv::Exception ex) {
		qDebug() << "OpenCV_pHash error:" << ex.msg.c_str();
	} catch (std::exception ex) {
		qDebug() << "OpenCV_pHash error:" << ex.what();
	} catch (int ex) {
		qDebug() << "OpenCV_pHash error:" << "Error:" << ex;
	}
	return false;
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;

	auto view = new QGraphicsView();
	auto scene = new QGraphicsScene(&w);
	view->setScene(scene);

	auto dir = QDir(IMAGE_DIR);

	auto list = dir.entryList(QDir::Filter::Files);

	QList<QPixmap> pics;
	int i = 0;
	for (auto file : list)
	{
		auto pixMap = QPixmap(IMAGE_DIR + file);
		if (!pixMap.isNull())
		{
			uint64_t hash = 0;
			if (OpenCV_pHash(pixMap.toImage(), hash))
				qDebug() << "pHash:" << hash;

			qreal x = (i%12) * 150;
			qreal y = (i/12) * 150;

			qDebug() << IMAGE_DIR + file;
			pixMap = pixMap.scaled(140, 140, Qt::KeepAspectRatio);
			auto rect = new QGraphicsRectItem(x,y,140,140);
			auto pic = new QGraphicsPixmapItem(pixMap, rect);
			scene->addItem(rect);
			pic->setPos((x+(rect->rect().width()/2))-(pic->boundingRect().width()/2),(y+(rect->rect().height()/2))-(pic->boundingRect().height()/2));
			i++;
		}
	}

	view->show();
	w.setCentralWidget(view);
	w.show();

#ifdef __linux__
	ClientDB* db = ClientDB::Open("../test.db");
#elif _WIN32
	ClientDB* db = ClientDB::Open("..\\test.db");
#else
	ClientDB* db = ClientDB::Open(Some path here);
#endif


	return a.exec();
}
