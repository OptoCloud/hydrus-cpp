#ifndef HYDRUSMEDIA_H
#define HYDRUSMEDIA_H

#include <QObject>
#include <QUuid>
#include <QUrl>
#include <QList>
#include <QString>
#include <QCryptographicHash>
#include <QLabel>

/// Loaded media item
class HydrusMedia : public QObject
{
	Q_OBJECT
protected:
    HydrusMedia();
    HydrusMedia(const QUuid& id, QObject* parent = nullptr);

    QUuid m_id;
	QUrl m_mediaLocation;

    QList<QString> m_tags;

    QCryptographicHash m_md5;
    QCryptographicHash m_sha1;
    QCryptographicHash m_sha256;
    QCryptographicHash m_sha512;
public:
	QUrl GetThumbnail() const;
	QUrl GetLocation() const;

	void ClearTags();
	void AddTag(const QString& tag);
	void RemoveTag(const QString& tag);
	QList<QString> GetTags() const;

	virtual void Load() = 0;
	virtual void UnLoad() = 0;

	virtual void Display(QLabel& label) = 0;
};

#endif // HYDRUSMEDIA_H
