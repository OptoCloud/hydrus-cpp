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
#include <sha256.h>
#include <QHash>

#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>

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

//static const QStringList SupportedImageFormatsQt = QImageReader::supportedImageFormats();
//static const QStringList SupportedVideosQt = QMovie::supportedFormats();
//static const QStringList SupportedAnimationsQt = QMovie::supportedFormats();
static const QStringList SupportedAudiosQt = {"*.mp3", "*.flac","*.ogg","*.wma"};
static const QStringList SupportedMiscsQt = {"*.swf","*.pdf","*.psd","*.zip","*.rar","*.7z"};

class Image : public QRunnable
{
public:
	Image(const QString& path)
	{
		this->path = path;
	}
	QString path;
	uint64_t phash;
	QByteArray sha256;

	void run()
	{
		// Open file
		QFile f(path);
		f.open(QFile::ReadOnly);
		if (!f.isOpen())
			return;
		auto data = f.readAll();
		f.close();

		// Parse as cv::Mat
		auto dataMat = cv::Mat(data.size(), 1, CV_8UC1, (char*)data.data());
		auto image = cv::imdecode(dataMat, cv::IMREAD_UNCHANGED);

		// Do processing
		qDebug() << "Processing" << path;
		Hashing::Sha256 hash;
		hash.Update(image.data, image.total() * image.elemSize());
		sha256 =  hash.result();
		if (!image.empty())
			phash = ImageUtils::PHash_Compute(image);
	}
};

struct Relation
{
	Relation(const QString& name1, const QString& name2, uint16_t dist)
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

#include <algorithm>
#include "hydrusthumbnailview.h"
int main(int argc, char **argv)
{

	QThreadPool threadPool;
	threadPool.setMaxThreadCount(QThread::idealThreadCount());

	QApplication a(argc, argv);
	QApplication::setApplicationName("Hydrus");
	QApplication::setApplicationVersion("1.0");

	MainWindow w;

	auto page = new HydrusThumbnailView(&w);

	// Generate test data
	QList<int64_t> li;
	for (int64_t i = 0; i < 10000; i++)
		li.append(i);
	std::random_shuffle(li.begin(), li.end());

	// Set random data
	page->SetItems(li.toSet());
	page->setBackgroundBrush(Qt::red);

	w.setCentralWidget(page);

	/*
#ifdef __linux__
	ClientDB* db = ClientDB::Open("../test.db");
#elif _WIN32
	ClientDB* db = ClientDB::Open("..\\test.db");
#else
	ClientDB* db = ClientDB::Open(Some path here);
#endif

	delete db;

	auto view = new QGraphicsView();
	auto scene = new QGraphicsScene(&w);

	view->setScene(scene);

	auto dir = QDir(IMAGE_DIR);
	QDir(QDir::currentPath()).mkdir("db");
	QDir(QDir::currentPath() + "/db").mkdir("thumbs");
	QDir(QDir::currentPath() + "/db").mkdir("files");

	auto files = dir.entryList(QDir::Filter::Files, QDir::SortFlag::Name); // HYDRUS_IMAGETYPES

	if (files.length() == 0)
		return EXIT_SUCCESS;

	int contentHeight = 125;
	int contentWidth = 150;
	int itemPadding = 5;

	int horItemCount = (w.width()-itemPadding)/contentWidth;
	int itemWidth = contentWidth+2;
	int itemHeight = contentHeight+2;

	QList<QPixmap> pics;
	int i = 0;

	qDebug() << "Queueing images...";

	QList<Image> images;
	for (QString file : files)
		images.append(Image(IMAGE_DIR + file));

	QElapsedTimer timer;
	timer.start();
	for (uint32_t i = 0; i < images.length();)
		threadPool.start(&images[i++]);

	qDebug() << "Processing...";
	threadPool.waitForDone();
	qDebug().nospace() << "Used " << timer.elapsed() << "ms to calculate phash for " << files.length() << " images";

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

	for (auto image : images)
	{
		QFile f(image.path);
		f.open(QFile::ReadOnly);
		if (!f.isOpen())
			continue;
		auto data = f.readAll();
		f.close();

		QImage pic = QImage::fromData(data);
		if (!pic.isNull())
		{
			qreal x = ((i%horItemCount)*(itemWidth+itemPadding))+itemPadding;
			qreal y = ((i/horItemCount)*(itemHeight+itemPadding))+itemPadding;

			qDebug() << "X:" << x;
			qDebug() << "Y:" << y;

			qDebug() << IMAGE_DIR + image.path << '\n';

			QPixmap thumbNail = QPixmap::fromImage(pic.scaled(itemWidth, itemHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));

			qDebug() << thumbNail.cacheKey();
			thumbNail.save("thumbs/" + QString::number(image.phash), "PNG");

			auto rect = new QGraphicsRectItem(x-1,y-1,itemWidth+1,itemHeight+1);
			auto pic = new QGraphicsPixmapItem(thumbNail, rect);
			scene->addItem(rect);
			pic->setPos((x+(itemWidth/2))-(pic->boundingRect().width()/2),(y+(itemHeight/2))-(pic->boundingRect().height()/2));
			i++;
		}
		else
			qDebug().nospace() << "Skipped " << IMAGE_DIR << image.path;
	}

	w.setCentralWidget(view);
	*/
	w.show();

	return a.exec();
}
