#define BOOST_TEST_MODULE CommandsTests
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include "commands.hpp"
#include "storage.hpp"
#include "time.hpp"

namespace losev {

extern BSTree<std::string, RunnerProfile> allProfiles;
extern RunnerProfile* currentProfile;

} // namespace losev

BOOST_AUTO_TEST_SUITE(new_profile_tests)

BOOST_AUTO_TEST_CASE(create_new_profile)
{
  losev::allProfiles.clear();
  losev::currentProfile = nullptr;

  std::stringstream input("password123\n");
  std::stringstream output;

  bool result = losev::newProfile(input, output, "test_user");

  BOOST_TEST(result == true);
  BOOST_TEST(losev::allProfiles.size() == 1);
  BOOST_TEST(losev::allProfiles.get("test_user").name == "test_user");
}

BOOST_AUTO_TEST_CASE(create_duplicate_profile)
{
  losev::allProfiles.clear();
  losev::currentProfile = nullptr;

  std::stringstream input("password123\n");
  std::stringstream output;

  losev::newProfile(input, output, "test_user");
  bool result = losev::newProfile(input, output, "test_user");

  BOOST_TEST(result == false);
  BOOST_TEST(losev::allProfiles.size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(profile_session_tests)

BOOST_AUTO_TEST_CASE(set_profile_success)
{
  losev::allProfiles.clear();
  losev::currentProfile = nullptr;

  std::stringstream createInput("pass123\n");
  std::stringstream createOutput;
  losev::newProfile(createInput, createOutput, "runner1");

  std::stringstream setInput("pass123\n");
  std::stringstream setOutput;
  bool result = losev::setProfile(setInput, setOutput, "runner1");

  BOOST_TEST(result == true);
  BOOST_TEST(losev::currentProfile != nullptr);
  BOOST_TEST(losev::currentProfile->name == "runner1");
}

BOOST_AUTO_TEST_CASE(set_profile_wrong_password)
{
  losev::allProfiles.clear();
  losev::currentProfile = nullptr;

  std::stringstream createInput("pass123\n");
  std::stringstream createOutput;
  losev::newProfile(createInput, createOutput, "runner1");

  std::stringstream setInput("wrongpass\n");
  std::stringstream setOutput;
  bool result = losev::setProfile(setInput, setOutput, "runner1");

  BOOST_TEST(result == false);
  BOOST_TEST(losev::currentProfile == nullptr);
}

BOOST_AUTO_TEST_CASE(set_profile_nonexistent)
{
  losev::allProfiles.clear();
  losev::currentProfile = nullptr;

  std::stringstream setInput("pass123\n");
  std::stringstream setOutput;
  bool result = losev::setProfile(setInput, setOutput, "nonexistent");

  BOOST_TEST(result == false);
  BOOST_TEST(losev::currentProfile == nullptr);
}

BOOST_AUTO_TEST_CASE(quit_when_in_profile)
{
  losev::allProfiles.clear();
  losev::currentProfile = nullptr;

  std::stringstream createInput("pass123\n");
  std::stringstream createOutput;
  losev::newProfile(createInput, createOutput, "runner1");

  std::stringstream setInput("pass123\n");
  std::stringstream setOutput;
  losev::setProfile(setInput, setOutput, "runner1");

  std::stringstream quitOutput;
  bool result = losev::quit(quitOutput);

  BOOST_TEST(result == true);
  BOOST_TEST(losev::currentProfile == nullptr);
}

BOOST_AUTO_TEST_CASE(quit_when_not_in_profile)
{
  losev::allProfiles.clear();
  losev::currentProfile = nullptr;

  std::stringstream quitOutput;
  bool result = losev::quit(quitOutput);

  BOOST_TEST(result == false);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(train_tests)

BOOST_AUTO_TEST_CASE(add_train_success)
{
  losev::allProfiles.clear();
  losev::currentProfile = nullptr;

  std::stringstream createInput("pass123\n");
  std::stringstream createOutput;
  losev::newProfile(createInput, createOutput, "runner1");

  std::stringstream setInput("pass123\n");
  std::stringstream setOutput;
  losev::setProfile(setInput, setOutput, "runner1");

  std::stringstream addOutput;
  bool result = losev::addTrain(addOutput, 10, "38:55");

  BOOST_TEST(result == true);
  BOOST_TEST(losev::currentProfile != nullptr);

  bool hasTraining = false;
  for (auto it = losev::currentProfile->trainings.begin();
       it != losev::currentProfile->trainings.end(); ++it)
  {
    if (it->distance == 10 && it->time == "38:55" && it->id == 1)
    {
      hasTraining = true;
    }
  }
  BOOST_TEST(hasTraining == true);
}

BOOST_AUTO_TEST_CASE(add_train_not_in_profile)
{
  losev::allProfiles.clear();
  losev::currentProfile = nullptr;

  std::stringstream addOutput;
  bool result = losev::addTrain(addOutput, 10, "38:55");

  BOOST_TEST(result == false);
}

BOOST_AUTO_TEST_CASE(add_train_invalid_time)
{
  losev::allProfiles.clear();
  losev::currentProfile = nullptr;

  std::stringstream createInput("pass123\n");
  std::stringstream createOutput;
  losev::newProfile(createInput, createOutput, "runner1");

  std::stringstream setInput("pass123\n");
  std::stringstream setOutput;
  losev::setProfile(setInput, setOutput, "runner1");

  std::stringstream addOutput;
  bool result = losev::addTrain(addOutput, 10, "invalid");

  BOOST_TEST(result == false);
}

BOOST_AUTO_TEST_CASE(show_train_all_distances)
{
  losev::allProfiles.clear();
  losev::currentProfile = nullptr;

  std::stringstream createInput("pass123\n");
  std::stringstream createOutput;
  losev::newProfile(createInput, createOutput, "runner1");

  std::stringstream setInput("pass123\n");
  std::stringstream setOutput;
  losev::setProfile(setInput, setOutput, "runner1");

  losev::addTrain(setOutput, 10, "38:55");
  losev::addTrain(setOutput, 21, "1:22:50");

  std::stringstream showOutput;
  losev::showTrain(showOutput);

  std::string output = showOutput.str();
  BOOST_TEST(output.find("10km") != std::string::npos);
  BOOST_TEST(output.find("38:55") != std::string::npos);
  BOOST_TEST(output.find("21km") != std::string::npos);
  BOOST_TEST(output.find("1:22:50") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(show_train_specific_distance)
{
  losev::allProfiles.clear();
  losev::currentProfile = nullptr;

  std::stringstream createInput("pass123\n");
  std::stringstream createOutput;
  losev::newProfile(createInput, createOutput, "runner1");

  std::stringstream setInput("pass123\n");
  std::stringstream setOutput;
  losev::setProfile(setInput, setOutput, "runner1");

  losev::addTrain(setOutput, 10, "38:55");
  losev::addTrain(setOutput, 21, "1:22:50");

  std::stringstream showOutput;
  losev::showTrain(showOutput, 10);

  std::string output = showOutput.str();
  BOOST_TEST(output.find("10km") != std::string::npos);
  BOOST_TEST(output.find("38:55") != std::string::npos);
  BOOST_TEST(output.find("21km") == std::string::npos);
}

BOOST_AUTO_TEST_CASE(show_train_not_in_profile)
{
  losev::allProfiles.clear();
  losev::currentProfile = nullptr;

  std::stringstream showOutput;
  losev::showTrain(showOutput);

  std::string output = showOutput.str();
  BOOST_TEST(output.find("not in the profile") != std::string::npos);
}

BOOST_AUTO_TEST_SUITE(route_tests)

BOOST_AUTO_TEST_CASE(find_route_basic)
{
  // Граф должен быть загружен перед тестом
  // Для теста создаём временный граф с несколькими городами
  losev::Graph testGraph("test");
  testGraph.addEdge("Paris", "Brussels", 265);
  testGraph.addEdge("Brussels", "Cologne", 190);
  testGraph.addEdge("Paris", "Lyon", 465);

  // Временно заменяем europeGraph для теста
  // (для реального теста нужно создать отдельный граф)

  BOOST_TEST(true);  // Заглушка
}

BOOST_AUTO_TEST_CASE(find_route_not_loaded)
{
  // Если граф не загружен, должна быть ошибка
  BOOST_TEST(true);  // Заглушка
}

BOOST_AUTO_TEST_SUITE_END()
