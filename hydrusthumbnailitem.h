#ifndef HYDRUSTHUMBNAILITEM_H
#define HYDRUSTHUMBNAILITEM_H

#include <QGraphicsPixmapItem>
#include <QUuid>
#include <QMovie>

class HydrusThumbnailItem : public QGraphicsPixmapItem
{
public:
    HydrusThumbnailItem(const QUuid& itemId);

	QUuid mediaId() const;
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
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
private:
	QUuid m_id;
	uint32_t m_row = 0;
	uint32_t m_col = 0;
	QMovie* m_animationPreview = nullptr; // nullptr for images, else for gifs/vids
};

#endif // HYDRUSTHUMBNAILITEM_H
