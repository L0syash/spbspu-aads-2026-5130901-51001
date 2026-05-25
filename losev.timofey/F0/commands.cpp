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

bool setProfile(std::istream& in, std::ostream& out, const std::string& name)
{
  RunnerProfile profile;

  try
  {
    profile = allProfiles.get(name);
  }
  catch (const std::out_of_range&)
  {
    out << "Such a runner does not exist\n";
    return false;
  }

  out << "Enter password:\n";
  std::string password;
  in >> password;

  std::string encrypted = xorEncrypt(password, 0x5A);

  if (encrypted != profile.encryptedPassword)
  {
    out << "Wrong password\n";
    return false;
  }

  currentProfile = &allProfiles.get(name);
  out << "You are in profile: " << name << "\n";

  return true;
}

bool quit(std::ostream& out)
{
  if (currentProfile == nullptr)
  {
    out << "You are not in the profile\n";
    return false;
  }

  out << "You have logged out of profile: " << currentProfile->name << "\n";
  currentProfile = nullptr;

  return true;
}

}
