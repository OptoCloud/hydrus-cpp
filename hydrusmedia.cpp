#include "hydrusmedia.h"

HydrusMedia::HydrusMedia(int64_t id, QObject* parent) : QObject(parent)
{
    m_id = id;
}
