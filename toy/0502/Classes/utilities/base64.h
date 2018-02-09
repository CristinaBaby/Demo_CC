#ifndef __proj_ios__base64__
#define __proj_ios__base64__

#include <string>

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

#endif
