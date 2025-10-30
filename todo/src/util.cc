#include "util.hpp"
#include <regex>

unsigned long long date2timeStamp(std::string &dateStr) {
  std::regex datePattern(R"(^(\d{4})/(\d{1,2})/(\d{1,2})$)");
  std::smatch matches;
  int year, month, day;
  if (!std::regex_match(dateStr, matches, datePattern)) {
    return 0;
  }

  try {
    year = std::stoi(matches[1].str());
    month = std::stoi(matches[2].str());
    day = std::stoi(matches[3].str());
  } catch (const std::exception &e) {
    return 0;
  }

  std::tm timeinfo = {};
  timeinfo.tm_year = year - 1900;
  timeinfo.tm_mon = month - 1;
  timeinfo.tm_mday = day;
  timeinfo.tm_hour = 0;
  timeinfo.tm_min = 0;
  timeinfo.tm_sec = 0;

  return std::mktime(&timeinfo);
}

std::string timeStamp2date(time_t timestamp) {
  std::tm *localTime = std::localtime(&timestamp);
  int year, month, day;
  year = localTime->tm_year + 1900;
  month = localTime->tm_mon + 1;
  day = localTime->tm_mday;

  std::stringstream ss;
  ss << year << '/' << month << '/' << day;
  return ss.str();
}
