#include "time.hpp"
#include <sstream>
#include <vector>

namespace losev {

int timeToSeconds(const std::string& time)
{
  int hours = 0;
  int minutes = 0;
  int seconds = 0;
  std::vector<int> parts;
  std::stringstream ss(time);
  std::string part;

  while (std::getline(ss, part, ':'))
  {
    parts.push_back(std::stoi(part));
  }

  if (parts.size() == 2)
  {
    minutes = parts[0];
    seconds = parts[1];
  }
  else if (parts.size() == 3)
  {
    hours = parts[0];
    minutes = parts[1];
    seconds = parts[2];
  }

  return hours * 3600 + minutes * 60 + seconds;
}

}
