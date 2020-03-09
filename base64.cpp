#include "base64.h"
#include <iostream>

static const std::string base64_chars =
			 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			 "abcdefghijklmnopqrstuvwxyz"
			 "0123456789+/";

ByteArray Base64::Encode(const ByteArray& array)
{
	return Encode(array.data, array.size);
}
ByteArray Base64::Decode(const ByteArray& array)
{
	return Decode(array.data, array.size);
}

ByteArray Base64::Encode(const std::string& string)
{
	return Encode(reinterpret_cast<const uint8_t*>(string.c_str()), string.size());
}
ByteArray Base64::Decode(const std::string& string)
{
	return Decode(reinterpret_cast<const uint8_t*>(string.c_str()), string.size());
}

ByteArray Base64::Encode(const uint8_t* data, uint32_t size) {
  ByteArray ret;
  int i = 0;
  int j = 0;
  unsigned char array_3[3];
  unsigned char array_4[4];

  while (size--) {
	array_3[i++] = *(data++);
	if (i == 3) {
	  array_4[0] = (array_3[0] & 0xfc) >> 2;
	  array_4[1] = ((array_3[0] & 0x03) << 4) + ((array_3[1] & 0xf0) >> 4);
	  array_4[2] = ((array_3[1] & 0x0f) << 2) + ((array_3[2] & 0xc0) >> 6);
	  array_4[3] = array_3[2] & 0x3f;

	  for(i = 0; (i <4) ; i++)
		ret += base64_chars[array_4[i]];
	  i = 0;
	}
  }

  if (i)
  {
	for(j = i; j < 3; j++)
	  array_3[j] = '\0';

	array_4[0] = ( array_3[0] & 0xfc) >> 2;
	array_4[1] = ((array_3[0] & 0x03) << 4) + ((array_3[1] & 0xf0) >> 4);
	array_4[2] = ((array_3[1] & 0x0f) << 2) + ((array_3[2] & 0xc0) >> 6);

	for (j = 0; (j < i + 1); j++)
	  ret += base64_chars[array_4[j]];

	while((i++ < 3))
	  ret += '=';

  }

  return ret;

}
ByteArray Base64::Decode(const uint8_t* data, uint32_t size) {
  int in_len = size;
  int i = 0;
  int j = 0;
  int in_ = 0;
  uint8_t array_4[4], array_3[3];
  ByteArray ret;

  while (in_len-- && ( data[in_] != '=') && IsBase64(data[in_])) {
	array_4[i++] = data[in_]; in_++;
	if (i ==4) {
	  for (i = 0; i <4; i++)
		array_4[i] = base64_chars.find(array_4[i]);

	  array_3[0] = ( array_4[0] << 2       ) + ((array_4[1] & 0x30) >> 4);
	  array_3[1] = ((array_4[1] & 0xf) << 4) + ((array_4[2] & 0x3c) >> 2);
	  array_3[2] = ((array_4[2] & 0x3) << 6) +   array_4[3];

	  for (i = 0; (i < 3); i++)
		ret += array_3[i];
	  i = 0;
	}
  }

  if (i) {
	for (j = 0; j < i; j++)
	  array_4[j] = base64_chars.find(array_4[j]);

	array_3[0] = (array_4[0] << 2) + ((array_4[1] & 0x30) >> 4);
	array_3[1] = ((array_4[1] & 0xf) << 4) + ((array_4[2] & 0x3c) >> 2);

	for (j = 0; (j < i - 1); j++) ret += array_3[j];
  }

  return ret;
}

bool Base64::IsBase64(char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}
bool Base64::IsBase64(uint8_t byte) {
  return (isalnum(byte) || (byte == '+') || (byte == '/'));
}
bool Base64::IsBase64(const ByteArray& array)
{
	for (uint32_t i = 0; i < array.size; i++)
		if (!isalnum(array.data[i]) && (array.data[i] != '+') && (array.data[i] != '/'))
			return false;
	return true;
}
bool Base64::IsBase64(const std::string& string)
{
	char* c = const_cast<char*>(string.c_str());
	for (uint32_t i = 0; i < string.size(); i++)
		if (!isalnum(c[i]) && (c[i] != '+') && (c[i] != '/'))
			return false;
	return true;
}
bool Base64::IsBase64(const uint8_t* data, uint32_t size)
{
	for (uint32_t i = 0; i < size; i++)
		if (!isalnum(data[i]) && (data[i] != '+') && (data[i] != '/'))
			return false;
	return true;
}
