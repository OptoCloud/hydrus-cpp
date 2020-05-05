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
	HydrusThumbnailView(QWidget* parent = nullptr);
	HydrusThumbnailView(QSize itemSize, int itemSpacing, QWidget* parent = nullptr);
	HydrusThumbnailView(int itemWidth, int itemHeight, int itemSpacing, QWidget* parent = nullptr);
	~HydrusThumbnailView();

	void SetItems(const QSet<int64_t>& ids);
	int AddItems(const QSet<int64_t>& ids);
	int RemoveItems(const QSet<int64_t>& ids);
	void ClearItems();
	QSet<int64_t> GetItems() const;
protected:
	void resizeEvent(QResizeEvent *event) override;
signals:
	void itemsChanged(const QSet<int64_t>& list);
	void itemFocused(int64_t id);
	void itemClicked(int64_t id);
	void itemLeftClicked(int64_t id);
	void itemDoubleClicked(int64_t id);
private slots:
	void PositionAndScaleItems(const QSize& viewSize);
	void PositionItems(const QSize& viewSize);
private:
	int m_itemWidth;
	int m_itemHeight;
	int m_itemSpacing;
	int m_prevColCount = 0;
	QGraphicsScene* m_scene;
	QMap<int64_t, HydrusThumbnailItem*> m_thumbnails;
};

#endif // HYDRUSMEDIAVIEW_H
