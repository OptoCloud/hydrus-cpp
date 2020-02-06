#ifndef HYDRUSIMAGESTILL_H
#define HYDRUSIMAGESTILL_H

#include <QPixmap>
#include "hydrusmedia.h"

class HydrusImage : public HydrusMedia
{
	Q_OBJECT
	QPixmap* m_imageData = nullptr; // Unloaded until played
public:
    HydrusImage(QPixmap& other, const QUuid& id, QObject* parent = nullptr);
};

#endif // HYDRUSIMAGESTILL_H
