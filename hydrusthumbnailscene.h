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
#include "hydrusid.h"
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

	void setItems(const QSet<HydrusID>& ids);
	int addItems(const QSet<HydrusID>& ids);
	int removeItems(const QSet<HydrusID>& ids);
	void clearItems();
	QSet<HydrusID> getItems() const;

	void setupViewport(QWidget *widget) override;
protected:
	void resizeEvent(QResizeEvent *event) override;
signals:
	void itemsChanged(const QSet<HydrusID>& list);
	void itemFocused(HydrusID id);
	void itemClicked(HydrusID id);
	void itemLeftClicked(HydrusID id);
	void itemDoubleClicked(HydrusID id);
private slots:
	void refreshView(const QRectF& view);
private:
	QSizeF m_itemSize;
	qreal  m_itemSpacing;
	QList<HydrusThumbnailItem*> m_data;
	QMap<HydrusID, int> m_idToIndexMap;
	QGraphicsScene* m_scene;
};

#endif // HYDRUSMEDIAVIEW_H
