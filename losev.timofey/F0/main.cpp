#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <functional>
#include "commands.hpp"
#include "storage.hpp"

namespace losev {

void handleNewProfile(std::istream& in, std::ostream& out, const std::string& arg)
{
  newProfile(in, out, arg);
}

void handleSetProfile(std::istream& in, std::ostream& out, const std::string& arg)
{
  setProfile(in, out, arg);
}

void handleQuit(std::istream&, std::ostream& out, const std::string&)
{
  quit(out);
}

} // namespace losev

int main()
{
  using namespace losev;

  loadData("runners.txt");

  std::unordered_map<std::string, std::function<void(std::istream&, std::ostream&, const std::string&)>> commands;

  commands["new-profile"] = handleNewProfile;
  commands["set-profile"] = handleSetProfile;
  commands["quit"] = handleQuit;

  std::string line;

  while (std::getline(std::cin, line))
  {
    if (line.empty())
    {
      continue;
    }

    std::stringstream ss(line);
    std::string cmd;
    ss >> cmd;

    std::string arg;
    ss >> arg;

    auto it = commands.find(cmd);
    if (it != commands.end())
    {
      it->second(std::cin, std::cout, arg);
    }
    else
    {
      std::cout << "INVALID COMMAND\n";
    }
  }

  saveData("runners.txt");

  return 0;
}
