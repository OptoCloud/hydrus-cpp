#ifndef HYDRUSITEM_H
#define HYDRUSITEM_H

#include <cstring>
#include <cstdint>
#include <stdlib.h>

class QString;

/**
 * @title HydrusID
 * @details Base32 12-byte id
 */
struct HydrusID
{
public:
	HydrusID()
	{
		memset(data, 0, 12);
	}
	HydrusID(const HydrusID& id)
	{
		memcpy(const_cast<uint32_t*>(id.data), this->data, 12);
	}
	HydrusID(const char* id)
	{
		for (int i = 0; i < 3; i++)
		{
			data[i]  = ((id[i * 8    ] - 'a') << 28) & 0xF0000000;
			data[i] |= ((id[i * 8 + 1] - 'a') << 24) & 0x0F000000;
			data[i] |= ((id[i * 8 + 2] - 'a') << 20) & 0x00F00000;
			data[i] |= ((id[i * 8 + 3] - 'a') << 16) & 0x000F0000;
			data[i] |= ((id[i * 8 + 4] - 'a') << 12) & 0x0000F000;
			data[i] |= ((id[i * 8 + 5] - 'a') <<  8) & 0x00000F00;
			data[i] |= ((id[i * 8 + 6] - 'a') <<  4) & 0x000000F0;
			data[i] |= ((id[i * 8 + 7] - 'a')      ) & 0x0000000F;
		}
	}
	HydrusID(const uint8_t* id)
	{
		memcpy(const_cast<uint8_t*>(id), this->data, 12);
	}

	void idCStr(char* c_str) const
	{
		for (int i = 0; i < 3; i++)
		{
			c_str[i * 8    ] = ((data[i] >> 28) & 0xF) + 'a';
			c_str[i * 8 + 1] = ((data[i] >> 24) & 0xF) + 'a';
			c_str[i * 8 + 2] = ((data[i] >> 20) & 0xF) + 'a';
			c_str[i * 8 + 3] = ((data[i] >> 16) & 0xF) + 'a';
			c_str[i * 8 + 4] = ((data[i] >> 12) & 0xF) + 'a';
			c_str[i * 8 + 5] = ((data[i] >>  8) & 0xF) + 'a';
			c_str[i * 8 + 6] = ((data[i] >>  4) & 0xF) + 'a';
			c_str[i * 8 + 7] = ((data[i]      ) & 0xF) + 'a';
		}
	}
	void idDat(uint8_t* data) const
	{
		memcpy(const_cast<uint32_t*>(this->data), data, 12);
	}

	bool operator==(const HydrusID& other) const
	{
		return memcmp(this->data, other.data, 12) == 0;
	}
	bool operator!=(const HydrusID& other) const
	{
		return memcmp(this->data, other.data, 12) != 0;
	}
	bool operator>(const HydrusID& other) const
	{
		return memcmp(this->data, other.data, 12) > 0;
	}
	bool operator>=(const HydrusID& other) const
	{
		return memcmp(this->data, other.data, 12) >= 0;
	}
	bool operator<(const HydrusID& other) const
	{
		return memcmp(this->data, other.data, 12) < 0;
	}
	bool operator<=(const HydrusID& other) const
	{
		return memcmp(this->data, other.data, 12) <= 0;
	}
private:
	uint32_t data[3];
};

#endif // HYDRUSITEM_H
