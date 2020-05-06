#ifndef HYDRUSMEDIAVIEW_H
#define HYDRUSMEDIAVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>

#include <QSet>
#include <QWidget>
#include <QBoxLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "hydrusthumbnailitem.h"

class HydrusThumbnailView : public QGraphicsView
{
	Q_OBJECT

public:
	HydrusThumbnailView(const QRectF& sceneRect, QWidget* parent = nullptr);
	HydrusThumbnailView(const QRectF& sceneRect, const QSizeF& itemSize, qreal itemSpacing, QWidget* parent = nullptr);
	HydrusThumbnailView(qreal scenePosX, qreal scenePosY, qreal sceneWidth, qreal sceneHeight, QWidget* parent = nullptr);
	HydrusThumbnailView(qreal scenePosX, qreal scenePosY, qreal sceneWidth, qreal sceneHeight, qreal itemWidth, qreal itemHeight, qreal itemSpacing, QWidget* parent = nullptr);
	~HydrusThumbnailView();

	void setItemLayout(const QSizeF& size, qreal margin);
	void setItemLayout(qreal widht, qreal height, qreal margin);
	QSizeF getItemSize();
	qreal getItemMargin();

	void setItems(const QSet<qint64>& ids);
	int addItems(const QSet<qint64>& ids);
	int removeItems(const QSet<qint64>& ids);
	void clearItems();
	QSet<qint64> getItems() const;
protected:
	void resizeEvent(QResizeEvent *event) override;
signals:
	void itemsChanged(const QSet<qint64>& list);
	void itemFocused(qint64 id);
	void itemClicked(qint64 id);
	void itemLeftClicked(qint64 id);
	void itemDoubleClicked(qint64 id);
private slots:
	void PositionAndScaleItems(const QSize& viewSize);
	void PositionItems(const QSize& viewSize);
private:
	QSizeF m_itemSize;
	qreal m_itemMargin;
	quint8 m_prevColCount = 0;
	QGraphicsScene* m_scene;
	QMap<qint64, HydrusThumbnailItem*> m_thumbnails;
};

#endif // HYDRUSMEDIAVIEW_H
