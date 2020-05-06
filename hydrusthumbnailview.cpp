#include "hydrusthumbnailview.h"

#include <QResizeEvent>

#define DEFAULT_WIDTH 150
#define DEFAULT_HEIGHT 125
#define DEFAULT_SPACING 5

HydrusThumbnailView::HydrusThumbnailView(const QRectF& sceneRect, QWidget* parent) :
	QGraphicsView(parent),
	m_itemSize(DEFAULT_WIDTH, DEFAULT_HEIGHT),
	m_itemMargin(DEFAULT_SPACING)
{
	m_scene = new QGraphicsScene(sceneRect, this);
	setScene(m_scene);
}
HydrusThumbnailView::HydrusThumbnailView(const QRectF& sceneRect, const QSizeF& itemSize, qreal itemSpacing, QWidget* parent) :
	QGraphicsView(parent),
	m_itemSize(itemSize),
	m_itemMargin(itemSpacing)
{
	m_scene = new QGraphicsScene(sceneRect, this);
	setScene(m_scene);
}
HydrusThumbnailView::HydrusThumbnailView(qreal scenePosX, qreal scenePosY, qreal sceneWidth, qreal sceneHeight, QWidget* parent) :
	QGraphicsView(parent),
	m_itemSize(DEFAULT_WIDTH, DEFAULT_HEIGHT),
	m_itemMargin(DEFAULT_SPACING)
{
	m_scene = new QGraphicsScene(scenePosX, scenePosY, sceneWidth, sceneHeight, this);
	setScene(m_scene);
}
HydrusThumbnailView::HydrusThumbnailView(qreal scenePosX, qreal scenePosY, qreal sceneWidth, qreal sceneHeight, qreal itemWidth, qreal itemHeight, qreal itemSpacing, QWidget* parent) :
	QGraphicsView(parent),
	m_itemSize(itemWidth, itemHeight),
	m_itemMargin(itemSpacing)
{
	m_scene = new QGraphicsScene(scenePosX, scenePosY, sceneWidth, sceneHeight, this);
	setScene(m_scene);
}

HydrusThumbnailView::~HydrusThumbnailView()
{
}

void HydrusThumbnailView::setItemLayout(const QSizeF& size, qreal margin)
{
	m_itemSize = size;
	m_itemMargin = margin;
}

void HydrusThumbnailView::setItemLayout(qreal width, qreal height, qreal margin)
{
	m_itemSize = QSizeF(width, height);
	m_itemMargin = margin;
}

QSizeF HydrusThumbnailView::getItemSize()
{
	return m_itemSize;
}

qreal HydrusThumbnailView::getItemMargin()
{
	return m_itemMargin;
}

void HydrusThumbnailView::setItems(const QSet<qint64>& ids)
{
	auto loadedIds = m_thumbnails.keys().toSet();
	auto idsToLoad = ids - loadedIds;

	// If none of the items we are about to load are already here, then clear the data
	if (idsToLoad.count() == ids.count())
	{
		m_scene->clear();
		m_thumbnails.clear();
	}
	else
	{
		// else remove all id's that arent in the new ids array
		for (qint64 i : (loadedIds - ids))
		{
			auto item = m_thumbnails.take(i);
			m_scene->removeItem(item);
			delete item;
		}
	}

	// Load all id's we dont already have loaded
	for (qint64 i : idsToLoad)
	{
		auto item = new HydrusThumbnailItem(i);
		item->setBrush(Qt::black);
		m_scene->addItem(item);
		m_thumbnails.insert(i, item);
	}

	// Notify of change, and recalculate layout
	PositionItems(this->size());
	itemsChanged(ids);
}
int HydrusThumbnailView::addItems(const QSet<qint64> &ids)
{
	auto loadedIds = m_thumbnails.keys().toSet();
	auto idsToLoad = ids - loadedIds;

	if (idsToLoad.isEmpty())
		return 0;

	// Load all id's we dont already have loaded
	for (qint64 i : idsToLoad)
	{
		auto item = new HydrusThumbnailItem(i);
		m_scene->addItem(item);
		m_thumbnails.insert(i, item);
	}

	// Notify of change, and recalculate layout
	PositionItems(this->size());
	itemsChanged(loadedIds + idsToLoad);

	return idsToLoad.count();
}
int HydrusThumbnailView::removeItems(const QSet<qint64> &ids)
{
	auto loadedIds = m_thumbnails.keys().toSet();
	auto idsToUnload = loadedIds & ids;

	if (idsToUnload.isEmpty())
		return 0;

	// If all the id's will be removed
	if (idsToUnload.count() == loadedIds.count())
	{
		// Clear the scene
		m_scene->clear();
	}
	else
	{
		// Remove all id's that are in the new ids array
		for (qint64 i : idsToUnload)
		{
			auto item = m_thumbnails.take(i);
			m_scene->removeItem(item);
			delete item;
		}
	}

	// Notify of change, and recalculate layout
	PositionItems(this->size());
	itemsChanged(loadedIds - idsToUnload);

	return idsToUnload.count();
}
void HydrusThumbnailView::clearItems()
{
	if (!m_thumbnails.isEmpty())
	{
		m_scene->clear();

		// Notify of change, and recalculate layout
		itemsChanged(m_thumbnails.keys().toSet());
	}
}
QSet<qint64> HydrusThumbnailView::getItems() const
{
	return m_thumbnails.keys().toSet();
}

void HydrusThumbnailView::resizeEvent(QResizeEvent* event)
{
	if (event->type() == QResizeEvent::Type::Resize)
	{
		PositionItems(event->size());
	}

	QGraphicsView::resizeEvent(event);
}

void HydrusThumbnailView::PositionAndScaleItems(const QSize& viewSize)
{
	// Column currently at, and number of columns to create
	quint8 col = 0;
	quint8 nCols = 1;

	// Item position (top-left corner)
	qreal posX = m_itemMargin;
	qreal posY = m_itemMargin;

	// Item position offset values
	qreal incValX = m_itemSize.width() + m_itemMargin;
	qreal incValY = m_itemSize.height() + m_itemMargin;

	// Minimum width of 1 row + margin
	qreal minWidth = incValX + m_itemMargin;

	// If width is more than minimum then how many columns can we fit within it?
	if (viewSize.width() > minWidth)
	{
		// Add 0.5 to round off properly
		m_prevColCount = nCols = ((viewSize.width() / incValX) + 0.5);
	}

	// Redraw all items
	for (auto item : m_thumbnails.values())
	{
		// Position and scale
		item->setRect(QRectF(QPointF(posX, posY), m_itemSize));

		// If the next column exceeds the amount columns theres space for, then go back to column position 0
		if (++col < nCols)
		{
			// increment column
			posX += incValX;
		}
		else
		{
			col = 0;
			// Set column to first, and increment row
			posX = m_itemMargin;
			posY += incValY;
		}
	}
}

void HydrusThumbnailView::PositionItems(const QSize& viewSize)
{
	// Column currently at, and number of columns to create
	quint8 col = 0;
	quint8 nCols = 1;

	// Item position (top-left corner)
	qreal posX = m_itemMargin;
	qreal posY = m_itemMargin;

	// Item position offset values
	qreal incValX = m_itemSize.width() + m_itemMargin;
	qreal incValY = m_itemSize.height() + m_itemMargin;

	// Minimum width of 1 row + margin
	qreal minWidth = incValX + m_itemMargin;

	// If width is more than minimum then how many columns can we fit within it?
	if (viewSize.width() > minWidth)
	{
		// Add 0.5 to round off properly
		nCols = ((viewSize.width() / incValX) + 0.5);
	}

	// If the layout changed
	if (nCols != m_prevColCount)
	{
		m_prevColCount = nCols;

		// Redraw all items
		for (auto item : m_thumbnails.values())
		{
			// Position
			item->setPos(posX, posY);

			// If the next column exceeds the amount columns theres space for, then go back to column position 0
			if (++col < nCols)
			{
				// increment column
				posX += incValX;
			}
			else
			{
				col = 0;
				// Set column to first, and increment row
				posX = m_itemMargin;
				posY += incValY;
			}
		}
	}
}
