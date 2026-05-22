#define BOOST_TEST_MODULE BSTreeTests
#include <boost/test/included/unit_test.hpp>
#include "bstree.hpp"

BOOST_AUTO_TEST_SUITE(bstree_tests)

BOOST_AUTO_TEST_CASE(empty_tree_test)
{
  losev::BSTree<int, std::string> tree;
  BOOST_TEST(tree.empty() == true);
  BOOST_TEST(tree.size() == 0);
}

BOOST_AUTO_TEST_SUITE_END()
