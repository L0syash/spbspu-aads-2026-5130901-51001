#define BOOST_TEST_MODULE CommandsTests
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include "commands.hpp"
#include "storage.hpp"

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
