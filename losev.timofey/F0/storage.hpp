#ifndef LOSEV_F0_STORAGE_HPP
#define LOSEV_F0_STORAGE_HPP

#include <string>
#include "../common/bstree.hpp"
#include "types.hpp"

namespace losev {

extern BSTree<std::string, RunnerProfile> allProfiles;
extern RunnerProfile* currentProfile;

void saveData(const std::string& filename);
void loadData(const std::string& filename);

}

#endif
