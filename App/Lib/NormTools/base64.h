

#ifndef _BASE64_H_
#define _BASE64_H_

#ifdef __cplusplus
#include <string>
std::string base64_encode(unsigned char const* s, unsigned int len);
std::string base64_decode(std::string const& s);

extern "C"{
	int base64_encode(unsigned char const *src, unsigned int Srclen, unsigned char *dest, unsigned int *destlen);
	int base64_decode(unsigned char const *src, unsigned int Srclen, unsigned char *dest, unsigned int *destlen);
}
#else

int base64_encode(unsigned char const *src, unsigned int Srclen, unsigned char *dest, unsigned int *destlen);
int base64_decode(unsigned char const *src, unsigned int Srclen, unsigned char *dest, unsigned int *destlen);

#endif

#endif