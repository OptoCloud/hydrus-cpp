#ifndef HYDRUSIMAGEANIMATED_H
#define HYDRUSIMAGEANIMATED_H

#include <QMovie>
#include "hydrusmedia.h"

class HydrusAnimation : public HydrusMedia
{
	Q_OBJECT
	QMovie* m_animData = nullptr; // Unloaded until mousehover/played
public:
    HydrusAnimation(const HydrusAnimation& other);
};

#endif // HYDRUSIMAGEANIMATED_H
