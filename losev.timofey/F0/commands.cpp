#include "commands.hpp"
#include "storage.hpp"
#include "crypto.hpp"
#include <stdexcept>

namespace losev {

bool newProfile(std::istream& in, std::ostream& out, const std::string& name)
{
  try
  {
    allProfiles.get(name);
    out << "Runner already exists\n";
    return false;
  }
  catch (const std::out_of_range&)
  {
    // Профиль не найден, можно создавать
  }

  out << "Set password:\n";
  std::string password;
  in >> password;

  RunnerProfile profile;
  profile.name = name;
  profile.encryptedPassword = xorEncrypt(password, 0x5A);
  profile.nextId = 1;

  allProfiles.push(name, profile);
  out << "Profile created\n";

  return true;
}

}
