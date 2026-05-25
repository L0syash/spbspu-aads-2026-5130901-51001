#ifndef LOSEV_F0_COMMANDS_HPP
#define LOSEV_F0_COMMANDS_HPP

#include <iostream>
#include <string>

namespace losev {

bool newProfile(std::istream& in, std::ostream& out, const std::string& name);
bool setProfile(std::istream& in, std::ostream& out, const std::string& name);
bool quit(std::ostream& out);
bool addTrain(std::ostream& out, int distance, const std::string& time);
void showTrain(std::ostream& out, int distance = -1);

}

#endif
