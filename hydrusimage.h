#ifndef HYDRUSIMAGESTILL_H
#define HYDRUSIMAGESTILL_H

#include <QPixmap>
#include "hydrusmedia.h"

class HydrusImage : public HydrusMedia
{
	Q_OBJECT

public:
	HydrusImage(QPixmap& other, int64_t id, QObject* parent = nullptr);
private:

	uint64_t pHash;
	QPixmap* m_imageData = nullptr; // Unloaded until played
};

#endif // HYDRUSIMAGESTILL_H
