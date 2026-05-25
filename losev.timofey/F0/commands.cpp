#include "commands.hpp"
#include "storage.hpp"
#include "crypto.hpp"
#include "time.hpp"
#include <stdexcept>
#include <algorithm>

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

bool addTrain(std::ostream& out, int distance, const std::string& time)
{
  if (currentProfile == nullptr)
  {
    out << "You are not in the profile\n";
    return false;
  }

  int timeSeconds;

  try
  {
    timeSeconds = timeToSeconds(time);
  }
  catch (const std::exception&)
  {
    out << "Invalid time format\n";
    return false;
  }

  Training newTrain;
  newTrain.id = currentProfile->nextId;
  newTrain.distance = distance;
  newTrain.time = time;
  newTrain.timeSeconds = timeSeconds;

  currentProfile->trainings.push_front(newTrain);
  currentProfile->nextId++;

  out << "The train was successfully added.\n";

  return true;
}

void showTrain(std::ostream& out, int distance)
{
  if (currentProfile == nullptr)
  {
    out << "You are not in the profile\n";
    return;
  }

  // Собираем тренировки в вектор для сортировки по дистанции
  std::vector<Training> trainings;
  for (auto it = currentProfile->trainings.begin();
       it != currentProfile->trainings.end(); ++it)
  {
    if (distance == -1 || it->distance == distance)
    {
      trainings.push_back(*it);
    }
  }

  if (trainings.empty())
  {
    out << "No trainings found\n";
    return;
  }

  // Сортируем по дистанции и по времени (лучший сначала)
  std::sort(trainings.begin(), trainings.end(),
    [](const Training& a, const Training& b)
    {
      if (a.distance != b.distance)
      {
        return a.distance < b.distance;
      }
      return a.timeSeconds < b.timeSeconds;
    });

  int currentDist = -1;
  bool firstDist = true;

  for (size_t i = 0; i < trainings.size(); ++i)
  {
    const Training& t = trainings[i];

    if (t.distance != currentDist)
    {
      currentDist = t.distance;
      firstDist = true;

      out << "=======================\n";
      out << "|" << currentDist << "km: | time: |  id: |\n";
      out << "|______|_______|______|\n";
    }

    if (firstDist)
    {
      out << "|top - |" << t.time;
      firstDist = false;
    }
    else
    {
      out << "|      |" << t.time;
    }

    // Выравнивание пробелами (упрощённо)
    if (t.time.length() < 8)
    {
      out << " ";
    }
    out << " | " << t.id << " |\n";
  }

  out << "=======================\n";
}

}
