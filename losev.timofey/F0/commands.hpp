#ifndef LOSEV_F0_COMMANDS_HPP
#define LOSEV_F0_COMMANDS_HPP

#include <iostream>
#include <string>

namespace losev {

bool newProfile(std::istream& in, std::ostream& out, const std::string& name);
bool setProfile(std::istream& in, std::ostream& out, const std::string& name);
bool quit(std::ostream& out);

}

#endif
