#include "hydrusmedia.h"

HydrusMedia::HydrusMedia(qint64 id, QObject* parent) : QObject(parent)
{
    m_id = id;
}
