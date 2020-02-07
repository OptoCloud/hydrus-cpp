#include "mainwindow.h"

#include <QApplication>
#include <QPushButton>

#include <QGraphicsView>
#include <QGraphicsScene>
#include "hydrusthumbnailitem.h"

#include <clientdb.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    MainWindow w;

#ifdef __linux__
	ClientDB* db = ClientDB::Open("../test.db");
#elif _WIN32
	ClientDB* db = ClientDB::Open("..\\test.db");
#else
	ClientDB* db = ClientDB::Open(Some path here);
#endif


    auto view = new QGraphicsView();
    auto scene = new QGraphicsScene(&w);
	view->setScene(scene);

	auto d = QPixmap("../test.jpg").scaled(1000, 1000, Qt::KeepAspectRatio);

	for (int x = 0; x < 3; ++x)
	{
		for (int y = 0; y < 3; ++y)
		{
			auto pic = new QGraphicsPixmapItem(d);
			scene->addItem(pic);
			pic->setPos(
						d.width() * x,
						d.height() * y
						);
		}
	}
    view->show();
	w.setCentralWidget(view);
	w.show();
	return a.exec();
}
