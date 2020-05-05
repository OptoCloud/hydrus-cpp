#include "hydrusthumbnailview.h"

#include <QResizeEvent>

#define DEFAULT_WIDTH 150
#define DEFAULT_HEIGHT 125
#define DEFAULT_SPACING 5

HydrusThumbnailView::HydrusThumbnailView(QWidget* parent) :
	QGraphicsView(parent),
	m_itemWidth(DEFAULT_WIDTH),
	m_itemHeight(DEFAULT_HEIGHT),
	m_itemSpacing(DEFAULT_SPACING)
{
	m_scene = new QGraphicsScene(this);
	setScene(m_scene);
}
HydrusThumbnailView::HydrusThumbnailView(QSize itemSize, int itemSpacing, QWidget* parent) :
	QGraphicsView(parent),
	m_itemWidth(itemSize.width()),
	m_itemHeight(itemSize.height()),
	m_itemSpacing(itemSpacing)
{
	m_scene = new QGraphicsScene(this);
	setScene(m_scene);
}
HydrusThumbnailView::HydrusThumbnailView(int itemWidth, int itemHeight, int itemSpacing, QWidget* parent) :
	QGraphicsView(parent),
	m_itemWidth(itemWidth),
	m_itemHeight(itemHeight),
	m_itemSpacing(itemSpacing)
{
	m_scene = new QGraphicsScene(this);
	setScene(m_scene);
}

HydrusThumbnailView::~HydrusThumbnailView()
{
}

void HydrusThumbnailView::SetItems(const QSet<int64_t>& ids)
{
	// Get already loaded id's, and see which id's we dont have loaded
	auto loadedIds = m_thumbnails.keys().toSet();
	auto idsToLoad = ids - loadedIds;

	// If input is empty / we dont have any of the input ids
	if (ids.isEmpty() || idsToLoad.count() == ids.count())
	{
		// Clear the scene
		m_scene->clear();
	}
	else
	{
		// Remove all id's that arent in the new ids array
		for (int64_t i : (loadedIds - ids))
		{
			auto item = m_thumbnails.take(i);
			m_scene->removeItem(item);
			delete item;
		}
	}

	// Load all id's we dont already have loaded
	for (int64_t i : idsToLoad)
	{
		auto item = new HydrusThumbnailItem(i);
		item->setBrush(Qt::black);
		m_scene->addItem(item);
		m_thumbnails.insert(i, item);
	}

	// Notify of change, and recalculate layout
	PositionItems(this->size());
	itemsChanged(m_thumbnails.keys().toSet());
}
int HydrusThumbnailView::AddItems(const QSet<int64_t> &ids)
{
	auto idsToLoad = ids - m_thumbnails.keys().toSet();

	if (idsToLoad.isEmpty())
		return 0;

	// Load all id's we dont already have loaded
	for (int64_t i : idsToLoad)
	{
		auto item = new HydrusThumbnailItem(i);
		m_scene->addItem(item);
		m_thumbnails.insert(i, item);
	}

	// Notify of change, and recalculate layout
	PositionItems(this->size());
	itemsChanged(m_thumbnails.keys().toSet());

	return idsToLoad.count();
}
int HydrusThumbnailView::RemoveItems(const QSet<int64_t> &ids)
{
	auto idsToUnload = ids & m_thumbnails.keys().toSet();

	if (idsToUnload.isEmpty())
		return 0;

	// If all the id's will be removed
	if (idsToUnload.count() == ids.count())
	{
		// Clear the scene
		m_scene->clear();
	}
	else
	{
		// Remove all id's that are in the new ids array
		for (int64_t i : idsToUnload)
		{
			auto item = m_thumbnails.take(i);
			m_scene->removeItem(item);
			delete item;
		}
	}

	// Notify of change, and recalculate layout
	PositionItems(this->size());
	itemsChanged(m_thumbnails.keys().toSet());

	return idsToUnload.count();
}
void HydrusThumbnailView::ClearItems()
{
	if (!m_thumbnails.isEmpty())
	{
		m_scene->clear();

		// Notify of change, and recalculate layout
		itemsChanged(m_thumbnails.keys().toSet());
	}
}
QSet<int64_t> HydrusThumbnailView::GetItems() const
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
	// How many columns fits in the window width? (minimum of 1)
	int colCount =  std::max(
						viewSize.width() - m_itemSpacing,
						m_itemWidth + (m_itemSpacing * 2)
						)
					/
					(m_itemWidth + m_itemSpacing);

	if (colCount != m_prevColCount)
	{
		int col = 0;
		int posX = m_itemSpacing;
		int posY = m_itemSpacing;
		int incValX = m_itemWidth + m_itemSpacing;
		int incValY = m_itemHeight + m_itemSpacing;

		for (auto item : m_thumbnails.values())
		{
			item->setRect(posX, posY, m_itemWidth, m_itemHeight);

			// If the next column exceeds the amount columns theres space for, then go back to column position 0
			// Also offset the coords here
			if (++col > colCount)
			{
				col = 0;
				posX = m_itemSpacing;
				posY += incValY;
			}
			else
			{
				posX += incValX;
			}
		}
	}
}

void HydrusThumbnailView::PositionItems(const QSize& viewSize)
{
	// How many columns fits in the window width? (minimum of 1)
	int colCount =  std::max(
						viewSize.width() - m_itemSpacing,
						m_itemWidth + (m_itemSpacing * 2)
						)
					/
					(m_itemWidth + m_itemSpacing);

	if (colCount != m_prevColCount)
	{
		int col = 0;
		int posX = m_itemSpacing;
		int posY = m_itemSpacing;
		int incValX = m_itemWidth + m_itemSpacing;
		int incValY = m_itemHeight + m_itemSpacing;

		for (auto item : m_thumbnails.values())
		{
			item->setPos(posX, posY);

			// If the next column exceeds the amount columns theres space for, then go back to column position 0
			// Also offset the coords here
			if (++col > colCount)
			{
				col = 0;
				posX = m_itemSpacing;
				posY += incValY;
			}
			else
			{
				posX += incValX;
			}
		}
	}
}
