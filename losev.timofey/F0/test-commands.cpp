#define BOOST_TEST_MODULE CommandsTests
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include "commands.hpp"
#include "storage.hpp"

namespace losev {

extern BSTree<std::string, RunnerProfile> allProfiles;
extern RunnerProfile* currentProfile;

}

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

BOOST_AUTO_TEST_SUITE(profile_session_tests)

BOOST_AUTO_TEST_CASE(set_profile_success)
{
  losev::allProfiles.clear();
  losev::currentProfile = nullptr;

  // Сначала создаём профиль
  std::stringstream createInput("pass123\n");
  std::stringstream createOutput;
  losev::newProfile(createInput, createOutput, "runner1");

  // Пробуем войти
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
