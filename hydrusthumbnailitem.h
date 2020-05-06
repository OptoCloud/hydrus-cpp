#ifndef HYDRUSTHUMBNAILITEM_H
#define HYDRUSTHUMBNAILITEM_H

#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>

#include <QSet>

class HydrusThumbnailItem : public QGraphicsRectItem
{
public:
	HydrusThumbnailItem(qint64 id, QGraphicsItem* parent = nullptr);
	~HydrusThumbnailItem();

	qint64 id() const;
	QList<qint64> tagIds() const;
protected:
	// Context menu
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

	// Item selection
	void focusInEvent(QFocusEvent *event) override;
	void focusOutEvent(QFocusEvent *event) override;

	// For starting previews etc.
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;

	// Keyboard events: Arrows, Return, Delete, Etc.
	void keyPressEvent(QKeyEvent *event) override;
	void keyReleaseEvent(QKeyEvent *event) override;

	// Mouse events
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;private:
private:
	qint64 m_id;
	QSet<qint64> m_tags;
	QGraphicsPixmapItem* m_thumbnail;
};

#endif // HYDRUSTHUMBNAILITEM_H
