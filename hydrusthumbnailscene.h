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
#include "hydrusitem.h"
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
	void setItemLayout(qreal width, qreal height, qreal margin);
	QSizeF getItemSize() const { return m_itemSize; }
	qreal  getItemSpacing() const { return m_itemSpacing; }

	void setItems(const QSet<CustomID>& ids);
	int addItems(const QSet<CustomID>& ids);
	int removeItems(const QSet<CustomID>& ids);
	void clearItems();
	QSet<CustomID> getItems() const;

	void setupViewport(QWidget *widget) override;
protected:
	void resizeEvent(QResizeEvent *event) override;
signals:
	void itemsChanged(const QSet<CustomID>& list);
	void itemFocused(CustomID id);
	void itemClicked(CustomID id);
	void itemLeftClicked(CustomID id);
	void itemDoubleClicked(CustomID id);
private slots:
	void refreshView(const QRectF& view);
private:
	QSizeF m_itemSize;
	qreal  m_itemSpacing;
	QList<HydrusThumbnailItem*> m_data;
	QMap<CustomID, int> m_idToIndexMap;
	QGraphicsScene* m_scene;
};

#endif // HYDRUSMEDIAVIEW_H
