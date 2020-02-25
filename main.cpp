#include "mainwindow.h"

#include <QApplication>
#include <QPushButton>

#include <QGraphicsView>
#include <QGraphicsScene>
#include "hydrusthumbnailitem.h"

#include <QDir>
#include <QCryptographicHash>
#include <QDebug>
#include <clientdb.h>
#include <qmath.h>

#include <opencv4/opencv2/imgcodecs.hpp>

#include "imageutils.h"

#define IMAGE_DIR "../sampleImages/"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	qDebug() << QDir::currentPath();

	auto view = new QGraphicsView();
	auto scene = new QGraphicsScene(&w);

	view->setScene(scene);

	auto dir = QDir("thumbs/");//(IMAGE_DIR);
	//QDir(QDir::currentPath()).mkdir("thumbs");
	qDebug()  << dir;

	auto list = dir.entryList(QDir::Filter::Files);

	int contentHeight = 125;
	int contentWidth = 150;
	int itemPadding = 5;

	int horItemCount = (w.width()-itemPadding)/contentWidth;
	int itemWidth = contentWidth+2;
	int itemHeight = contentHeight+2;

	QList<QPixmap> pics;
	int i = 0;
	for (auto file : list)
	{

		auto image = QImage("thumbs/"/*IMAGE_DIR*/ + file);
		if (!image.isNull())
		{
			auto cvImg = ImageUtils::QImageToCVMat(image);
			//uint64_t pHash = ImageUtils::PHash(cvImg);
			//qDebug() << pHash;

			qreal x = ((i%horItemCount)*(itemWidth+itemPadding))+itemPadding;
			qreal y = ((i/horItemCount)*(itemHeight+itemPadding))+itemPadding;

			qDebug() << "X:" << x;
			qDebug() << "Y:" << y;

			qDebug() << IMAGE_DIR + file << '\n';

			QPixmap thumbNail = QPixmap::fromImage(image.scaled(itemWidth, itemHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));

			//qDebug() << pixMap.cacheKey();
			//pixMap.save("thumbs/" + QString::number(pixMap.cacheKey()), "PNG");

			auto rect = new QGraphicsRectItem(x-1,y-1,itemWidth+1,itemHeight+1);
			auto pic = new QGraphicsPixmapItem(thumbNail, rect);
			scene->addItem(rect);
			pic->setPos((x+(itemWidth/2))-(pic->boundingRect().width()/2),(y+(itemHeight/2))-(pic->boundingRect().height()/2));
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

// Different ways to read QImage, QPixmap, and cv::Mat at the same time (BENCHMARK THESE)
bool method1(const QString& fileName, const char *format = nullptr)
{
	// Open file
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
		return false;

	// Get file data
	QByteArray byteArray = file.readAll();
	file.close();

	// Get QImage
	QImage qImage = QImage::fromData(byteArray, format);
	if (qImage.isNull())
		return false;

	// Get QPixmap
	QPixmap qPixmap = QPixmap::fromImage(qImage);

	// Get cv::Mat
	cv::Mat cvImage = cv::imdecode(cv::Mat(1, byteArray.size(), CV_8UC3, byteArray.data()), cv::IMREAD_UNCHANGED);
	if (cvImage.data == nullptr)
		return false;
}

bool method2(const QString& fileName, const char *format = nullptr)
{
	// Get QImage
	QImage qImage = QImage(fileName, format);
	if (qImage.isNull())
		return false;

	// Get QPixmap
	QPixmap qPixmap = QPixmap::fromImage(qImage);

	// Get cv::Mat
	cv::Mat cvImage;
	if (qImage.format() == QImage::Format_RGB888) {
		cvImage = cv::Mat(qImage.height(), qImage.width(), CV_8UC3, (uint8_t*)qImage.constBits(), qImage.bytesPerLine());
	} else {
		QImage rgb888 = qImage.convertToFormat(QImage::Format_RGB888);
		cvImage = cv::Mat(rgb888.height(), rgb888.width(), CV_8UC3, (uint8_t*)rgb888.constBits(), rgb888.bytesPerLine());
	}
}
