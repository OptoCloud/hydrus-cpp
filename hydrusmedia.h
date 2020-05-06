#ifndef HYDRUSMEDIA_H
#define HYDRUSMEDIA_H

#include <QObject>
#include <QUrl>
#include <QSet>
#include <QString>
#include <QCryptographicHash>
#include <QLabel>

/// Loaded media item
class HydrusMedia : public QObject
{
	Q_OBJECT

protected:
	HydrusMedia();
	HydrusMedia(qint64 id, QObject* parent = nullptr);
public:
	QUrl getThumbnail() const;
	QUrl getLocation() const;

	void clearTags();
	void addTag(const qint64& tag);
	void removeTag(const qint64& tag);
	QSet<qint64> getTags() const;

	virtual void load() = 0;
	virtual void unLoad() = 0;

	virtual void display(QLabel& label) = 0;
protected:
	qint64 m_id;
	QUrl m_mediaLocation;

	QSet<qint64> m_tags;

	quint8 m_md5[16];
	quint8 m_sha1[20];
	quint8 m_sha256[32];
};

#endif // HYDRUSMEDIA_H
