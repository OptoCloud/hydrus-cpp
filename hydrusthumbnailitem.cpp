#include "hydrusthumbnailitem.h"

HydrusThumbnailItem::HydrusThumbnailItem(qint64 id, QGraphicsItem* parent) : QGraphicsRectItem(parent)
{
	m_id = id;
	m_thumbnail = new QGraphicsPixmapItem(this);
	m_thumbnail->setCacheMode(CacheMode::DeviceCoordinateCache);
}

HydrusThumbnailItem::~HydrusThumbnailItem()
{

}

qint64 HydrusThumbnailItem::id() const
{
	return m_id;
}

void HydrusThumbnailItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{

}

void HydrusThumbnailItem::focusInEvent(QFocusEvent* event)
{

}

void HydrusThumbnailItem::focusOutEvent(QFocusEvent* event)
{

}

void HydrusThumbnailItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{

}

void HydrusThumbnailItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{

}

void HydrusThumbnailItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{

}

void HydrusThumbnailItem::keyPressEvent(QKeyEvent* event)
{

}

void HydrusThumbnailItem::keyReleaseEvent(QKeyEvent* event)
{

}

void HydrusThumbnailItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{

}

void HydrusThumbnailItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{

}

void HydrusThumbnailItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{

}

void HydrusThumbnailItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{

}
