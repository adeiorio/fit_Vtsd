#ifndef STAT_STRINGUTILS_H
#define STAT_STRINGUTILS_H
#include <string>

std::string trim(const std::string& str,
                 const std::string& whitespace = " \t");

bool beginsWith(const std::string& str,
		const std::string& prefix);

#endif
