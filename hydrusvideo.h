#ifndef HYDRUSVIDEO_H
#define HYDRUSVIDEO_H

#include "hydrusmedia.h"

class HydrusVideo : public HydrusMedia
{
	Q_OBJECT
public:
	HydrusVideo(int64_t id, QObject* parent = nullptr);
};

#endif // HYDRUSVIDEO_H
