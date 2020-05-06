#include <QApplication>
#include <QSet>

#include "hydrusthumbnailview.h"

int main(int argc, char **argv)
{
	QApplication a(argc, argv);
	QApplication::setDesktopFileName("Hydrus");
	QApplication::setApplicationName("Hydrus");
	QApplication::setApplicationVersion("1.0");

	auto page = new HydrusThumbnailView(0, 0, 800, 400);

	// Generate test data
	QSet<qint64> fakeIdList;
	for (qint64 i = 0; i < 10000; i++)
		fakeIdList.insert(i);

	// Set random data
	page->setItems(fakeIdList);
	page->setBackgroundBrush(Qt::red);

	page->show();

	return a.exec();
}
