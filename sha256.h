#ifndef SHA256_H
#define SHA256_H

#include <cstdint>
#include <QString>
#include <QByteArray>

namespace Hashing {

#define SHA256_BLOCK_SIZE  ( 512 / 8)

class Sha256 final
{
	void transform(const quint8 *message, quint32 block_nb);
public:
	Sha256();
	void update(const quint8 *message, quint32 m_len);
	QByteArray result();
	QString hex();
	QString b64();
private:
	quint8  m_block[2 * SHA256_BLOCK_SIZE];
	quint32 m_h[8];
	quint32 m_len;
	quint32 m_totLen;
};

}

#endif // SHA256_H
