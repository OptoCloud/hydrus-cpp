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
#include <QElapsedTimer>
#include <QThread>
#include <QThreadPool>
#include <QMutex>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include "imageutils.h"

#define IMAGE_DIR "../sampleImages/"

static const QStringList SupportedImageFormatsOpenCV = {
	"*.bmp","*.dib",			// Windows bitmaps
	"*.jpeg","*.jpg","*.jpe",	// JPEG files
	"*.jp2",					// JPEG 2000 files
	"*.png",					// Portable Network Graphics
	"*.pbm","*.pgm","*.ppm",	// Portable image format
	"*.sr","*.ras",				// Sun rasters
	"*.tiff","*.tif"			// TIFF files
};
//static const QStringList SupportedImagesQt = QImageReader::supportedImageFormats();
//static const QStringList SupportedVideosQt = QMovie::supportedFormats();
//static const QStringList SupportedAnimationsQt = QMovie::supportedFormats();
static const QStringList SupportedAudiosQt = {"*.mp3", "*.flac","*.ogg","*.wma"};
static const QStringList SupportedMiscsQt = {"*.swf","*.pdf","*.psd","*.zip","*.rar","*.7z"};

class Image : public QRunnable
{
public:
	Image(QString path)
	{
		this->path = path;
	}
	QString path;
	uint64_t phash;
	QByteArray sha256;

	void run()
	{
		QFile f(path);
		f.open(QFile::ReadOnly);
		if (!f.isOpen())
			return;
		QByteArray arr = f.readAll();
		f.close();

		QImage image = QImage::fromData(arr);
		if (!image.isNull())
		{
			qDebug() << "Processing" << path;
			PHashCust(arr, phash);
			sha256 = ImageUtils::Sha256_Compute(image);
		}
	}
};

struct Relation
{
	Relation(QString& name1, QString& name2, uint16_t& dist)
	{
		this->name1 = name1;
		this->name2 = name2;
		this->dist = dist;
	}
	QString name1;
	QString name2;
	uint16_t dist;
};

bool compareRelations(const Relation &v1, const Relation &v2)
{
	return v1.dist < v2.dist;
}

int main(int argc, char **argv)
{
	int nproc = QThread::idealThreadCount();
	QThreadPool threadPool;
	threadPool.setMaxThreadCount(nproc);

	QApplication a(argc, argv);
	QApplication::setApplicationName("Hydrus");
	QApplication::setApplicationVersion("1.0");

	MainWindow w;

	auto view = new QGraphicsView();
	auto scene = new QGraphicsScene(&w);

	view->setScene(scene);

	auto dir = QDir(IMAGE_DIR);
	QDir(QDir::currentPath()).mkdir("db");
	QDir(QDir::currentPath() + "/db").mkdir("thumbs");
	QDir(QDir::currentPath() + "/db").mkdir("files");

	auto list = dir.entryList(QDir::Filter::Files, QDir::SortFlag::Name); // HYDRUS_IMAGETYPES

	if (list.length() == 0)
		return EXIT_SUCCESS;

	int contentHeight = 125;
	int contentWidth = 150;
	int itemPadding = 5;

	int horItemCount = (w.width()-itemPadding)/contentWidth;
	int itemWidth = contentWidth+2;
	int itemHeight = contentHeight+2;

	QList<QPixmap> pics;
	int i = 0;
	QElapsedTimer timer;
	timer.start();

	qDebug() << "Queueing images...";

	QList<Image> images;
	for (uint32_t i = 0; i < list.length(); ++i)
	{
		images.append(Image(IMAGE_DIR + list[i]));
	}

	for (uint32_t i = 0; i < images.length(); ++i)
	{
		threadPool.start((QRunnable*)&images[i]);
	}

	qDebug() << "Processing...";
	threadPool.waitForDone();

	QList<Relation> relations;
	qDebug() << "Computing similarities...";
	for (uint16_t x = 0; x < images.length(); ++x)
	{
		for (uint16_t y = images.length()-1; x < y; --y)
		{
			uint16_t distance = ImageUtils::HammingDistance(images[x].phash, images[y].phash);
			relations.push_back(Relation(images[x].path, images[y].path, distance));

			printf("Comparing %i to %i, resulted in a distance of %i\n", x, y, distance);
			fflush(stdout);
		}
	}

	qDebug() << "Sorting similarities...";
	qSort(relations.begin(), relations.end(), compareRelations);

	i = 0;
	qDebug() << "Displaying 100 top results:";
	for (auto relation : relations)
	{
		qDebug().nospace() << "Similarity is " << relation.dist << " between " << relation.name1 << " and " << relation.name2;
		i++;
		if (i >= 100)
			break;
	}



	uint64_t lastHash = 0;
	for (auto file : list)
	{
		QImage qtImage(IMAGE_DIR + file);
		if (!qtImage.isNull())
		{
			uint64_t pHash = ImageUtils::PHash_Compute(qtImage);

			qDebug() << "pHash:   " << pHash;
			qDebug() << "Distance:" << ImageUtils::HammingDistance(pHash, lastHash);
			lastHash = pHash;

			qreal x = ((i%horItemCount)*(itemWidth+itemPadding))+itemPadding;
			qreal y = ((i/horItemCount)*(itemHeight+itemPadding))+itemPadding;

			qDebug() << "X:" << x;
			qDebug() << "Y:" << y;

			qDebug() << IMAGE_DIR + file << '\n';

			QPixmap thumbNail = QPixmap::fromImage(qtImage.scaled(itemWidth, itemHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));

			//qDebug() << pixMap.cacheKey();
			//pixMap.save("thumbs/" + QString::number(pixMap.cacheKey()), "PNG");

			auto rect = new QGraphicsRectItem(x-1,y-1,itemWidth+1,itemHeight+1);
			auto pic = new QGraphicsPixmapItem(thumbNail, rect);
			scene->addItem(rect);
			pic->setPos((x+(itemWidth/2))-(pic->boundingRect().width()/2),(y+(itemHeight/2))-(pic->boundingRect().height()/2));
			i++;
		}
		else
			qDebug().nospace() << "Skipped " << IMAGE_DIR << file;
	}
	qDebug().nospace() << "Used " << timer.elapsed() << "ms to open " << list.length() << " images";

	view->show();
	w.setCentralWidget(view);
	w.show();
/*
#ifdef __linux__
	ClientDB* db = ClientDB::Open("../test.db");
#elif _WIN32
	ClientDB* db = ClientDB::Open("..\\test.db");
#else
	ClientDB* db = ClientDB::Open(Some path here);
#endif

	if (db != nullptr)
	{
		QSqlQuery query = db.exec("create table person (id integer primary key, firstname varchar(20), lastname varchar(30), age integer)");
	}
*/

	return a.exec();
}
