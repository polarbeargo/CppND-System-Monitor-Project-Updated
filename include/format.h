#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string Pad(std::string s, char c);
std::string ElapsedTime(long times);  // TODO: See src/format.cpp
};                                    // namespace Format

#endif