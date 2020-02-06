#include "hydrusmedia.h"

HydrusMedia::HydrusMedia(const QUuid& id, QObject* parent) : QObject(parent), m_md5(QCryptographicHash::Md5), m_sha1(QCryptographicHash::Sha1), m_sha256(QCryptographicHash::Sha256), m_sha512(QCryptographicHash::Sha512)
{
    m_id = id;
}
