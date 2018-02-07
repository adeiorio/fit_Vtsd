#include <string>

std::string trim(const std::string& str,
                 const std::string& whitespace) {
  const auto strBegin = str.find_first_not_of(whitespace);
  if (strBegin == std::string::npos) return "";
  const auto strEnd = str.find_last_not_of(whitespace);
  const auto strRange = strEnd - strBegin + 1;
  return str.substr(strBegin, strRange);
}

bool beginsWith(const std::string& str,
		const std::string& prefix) {
  return str.substr(0, prefix.size()) == prefix;
}
