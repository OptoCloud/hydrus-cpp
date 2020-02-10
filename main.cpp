#include "mainwindow.h"

#include <QApplication>
#include <QPushButton>

#include <QGraphicsView>
#include <QGraphicsScene>
#include "hydrusthumbnailitem.h"

#include <clientdb.h>
#include <QDir>
#include <QDebug>
#include <QElapsedTimer>

#include "libs/pHash.h"

static CImg<float> ph_dct_matrix(const int N) {
	CImg<float> matrix(N, N, 1, 1, 1 / sqrt((float)N));
	const float c1 = sqrt(2.0 / N);
	for (int x = 0; x < N; x++) {
		for (int y = 1; y < N; y++) {
			matrix(x, y) = c1 * cos((cimg::PI / 2 / N) * y * (2 * x + 1));
		}
	}
	return matrix;
}
static const CImg<float> dct_matrix = ph_dct_matrix(32);
int ph_dct_cimghash(CImg<uint8> image, ulong64 &hash) {

	CImg<float> meanfilter(7, 7, 1, 1, 1);
	CImg<float> img;
	if (image.spectrum() == 3) {
		img = image.RGBtoYCbCr().channel(0).get_convolve(meanfilter);
	} else if (image.spectrum() == 4) {
		int width = image.width();
		int height = image.height();
		img = image.crop(0, 0, 0, 0, width - 1, height - 1, 0, 2)
				  .RGBtoYCbCr()
				  .channel(0)
				  .get_convolve(meanfilter);
	} else {
		img = image.channel(0).get_convolve(meanfilter);
	}

	img.resize(32, 32);
	const CImg<float> &C = dct_matrix;
	CImg<float> Ctransp = C.get_transpose();

	CImg<float> dctImage = (C)*img * Ctransp;

	CImg<float> subsec = dctImage.crop(1, 1, 8, 8).unroll('x');

	float median = subsec.median();
	hash = 0;
	for (int i = 0; i < 64; i++, hash <<= 1) {
		float current = subsec(i);
		if (current > median) hash |= 0x01;
	}

	return 0;
}
CImg<uint8_t> QImage2CImg(const QImage& img)
{
	uint32_t h = img.height();
	uint32_t w = img.width();

	CImg<uint8_t> ret(w, h, 1, 3);

	for(uint32_t y = 0; y < h; ++y)
	{
		for(uint32_t x = 0; x < w; ++x)
		{
			QColor cin = img.pixelColor(x, y);
			uint8_t cout[3];
			cout[0] = cin.red();
			cout[1] = cin.green();
			cout[2] = cin.blue();
			ret.draw_point(x,y,0,cout,1);
		}
	}

	return ret;
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;

	auto view = new QGraphicsView();
	auto scene = new QGraphicsScene(&w);
	view->setScene(scene);

	ulong hash = 0;
	qDebug() << ph_dct_imagehash("..\\test.jpg", hash);
	qDebug() << hash;

	auto dir = QDir("../"); //"D:\\MEDIA LIBRARY\\nature\\");

	auto list = dir.entryList(QDir::Filter::Files);

	QList<QPixmap> pics;
	int i = 0;
	for (auto file : list)
	{
		auto pixMap = QPixmap("D:\\MEDIA LIBRARY\\nature\\" + file);
		if (!pixMap.isNull())
		{
			qreal x = (i%12) * 150;
			qreal y = (i/12) * 150;

			qDebug() << "D:\\MEDIA LIBRARY\\nature\\" + file;
			pixMap = pixMap.scaled(140, 140, Qt::KeepAspectRatio);
			auto rect = new QGraphicsRectItem(x,y,140,140);
			auto pic = new QGraphicsPixmapItem(pixMap, rect);
			scene->addItem(rect);
			pic->setPos((x+(rect->rect().width()/2))-(pic->boundingRect().width()/2),(y+(rect->rect().height()/2))-(pic->boundingRect().height()/2));

			ph_dct_cimghash(QImage2CImg(pixMap.toImage()), hash);
			qDebug() << hash;
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
