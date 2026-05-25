#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <unordered_map>

void add_string(std::istream& in, std::ostream&, std::vector< std::string >& db)
{
  std::string str;
  std::cin >> str;
  db.push_back(str);
}

void show_last(std::istream&, std::ostream&, std::vector< std::string >& db)
{
  std::cout << db.back() << "\n";
}

int main()
{
  std::vector< std::string > db;
  using cmd_t = void(*)(std::istream&, std::ostream&, std::vector< std::string >&);
  std::unordered_map< std::string, cmd_t > cmds;
  cmds["add"] = add_string;
  cmds["show_last"] = show_last;
  std::string cmd;
  while(std::cin >> cmd) {
    try {
      cmds.at(cmd)(std::cin, std::cout, db);
    } catch (const std::out_of_range&) {
      std::cout << "INVALID COMMAND\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    } catch (const std::logic_error& e) {
      std::cout << "<INVALID COMMAND: " << e.what() << ">\n";
    }
  }
  if (!std::cin.eof()) {
    std::cerr << "Bad input\n";
    return 1;
  }

  return 0;	
}
