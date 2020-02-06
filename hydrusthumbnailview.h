#ifndef HYDRUSMEDIAVIEW_H
#define HYDRUSMEDIAVIEW_H

#include <QListView>
#include <QList>
#include <QString>
#include "hydrusthumbnailitem.h"

class HydrusThumbnailView : QListView
{
public:
    HydrusThumbnailView();
private:
    QList<HydrusThumbnailItem> m_mediaElements;
};

#endif // HYDRUSMEDIAVIEW_H
