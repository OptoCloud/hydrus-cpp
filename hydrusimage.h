#ifndef HYDRUSIMAGESTILL_H
#define HYDRUSIMAGESTILL_H

#include <QPixmap>
#include "hydrusmedia.h"

class HydrusImage : public HydrusMedia
{
	Q_OBJECT

public:
	HydrusImage(QPixmap& other, qint64 id, QObject* parent = nullptr);

private:
	quint64 m_pHash;
	QPixmap* m_imageData = nullptr; // Unloaded until played
};

#endif // HYDRUSIMAGESTILL_H
