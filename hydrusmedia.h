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
	HydrusMedia(int64_t id, QObject* parent = nullptr);

	int64_t m_id;
	QUrl m_mediaLocation;

	QList<int64_t> m_tags;

	uint8_t m_md5[16];
	uint8_t m_sha1[20];
	uint8_t m_sha256[32];
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
