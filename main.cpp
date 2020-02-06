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

    ClientDB db("..\\test.db");

    auto view = new QGraphicsView();
    auto scene = new QGraphicsScene(&w);
    view->setScene(scene);
    auto i = new QGraphicsPixmapItem(QPixmap("..\\test.png"));
    scene->addItem(i);
    view->show();

	w.show();
	return a.exec();
}
