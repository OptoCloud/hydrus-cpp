#ifndef HYDRUSIMAGEANIMATED_H
#define HYDRUSIMAGEANIMATED_H

#include <QMovie>
#include "hydrusmedia.h"

class HydrusAnimation : public HydrusMedia
{
	Q_OBJECT
public:
    HydrusAnimation(const HydrusAnimation& other);
private:
	QMovie* m_animData = nullptr; // Unloaded until mousehover/played
};

#endif // HYDRUSIMAGEANIMATED_H
