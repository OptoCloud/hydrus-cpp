#ifndef HYDRUSVIDEO_H
#define HYDRUSVIDEO_H

#include "hydrusmedia.h"

class HydrusVideo : public HydrusMedia
{
	Q_OBJECT
public:
	HydrusVideo(qint64 id, QObject* parent = nullptr);
private:

};

#endif // HYDRUSVIDEO_H
