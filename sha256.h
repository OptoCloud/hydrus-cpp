#ifndef SHA256_H
#define SHA256_H

#include <cstdint>
#include <QString>
#include <QByteArray>

namespace Hashing {

#define SHA256_BLOCK_SIZE  ( 512 / 8)

class Sha256 final
{
	uint32_t tot_len;
	uint32_t len;
	uint8_t  block[2 * SHA256_BLOCK_SIZE];
	uint32_t h[8];

	void Transform(const uint8_t *message, uint32_t block_nb);
public:
	Sha256();
	void Update(const uint8_t *message, uint32_t len);
	QByteArray result();
	QString hex();
	QString b64();
};

}

#endif // SHA256_H
