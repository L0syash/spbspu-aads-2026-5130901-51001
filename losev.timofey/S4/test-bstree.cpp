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

BOOST_AUTO_TEST_CASE(push_and_get_test)
{
  losev::BSTree<int, std::string> tree;
  tree.push(5, "five");
  BOOST_TEST(tree.empty() == false);
  BOOST_TEST(tree.size() == 1);
  BOOST_TEST(tree.get(5) == "five");
}

BOOST_AUTO_TEST_CASE(push_multiple_test)
{
  losev::BSTree<int, std::string> tree;
  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");
  BOOST_TEST(tree.size() == 3);
  BOOST_TEST(tree.get(10) == "ten");
  BOOST_TEST(tree.get(5) == "five");
  BOOST_TEST(tree.get(15) == "fifteen");
}

BOOST_AUTO_TEST_CASE(overwrite_test)
{
  losev::BSTree<int, std::string> tree;
  tree.push(5, "five");
  tree.push(5, "cinq");
  BOOST_TEST(tree.size() == 1);
  BOOST_TEST(tree.get(5) == "cinq");
}

BOOST_AUTO_TEST_CASE(iterator_begin_end_test)
{
  losev::BSTree<int, std::string> tree;
  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");
  tree.push(3, "three");
  tree.push(7, "seven");

  auto it = tree.begin();
  BOOST_TEST(it != tree.end());
  it = tree.begin();
  BOOST_TEST((*it).first == 3);
  ++it;
  BOOST_TEST((*it).first == 5);
  ++it;
  BOOST_TEST((*it).first == 7);
  ++it;
  BOOST_TEST((*it).first == 10);
  ++it;
  BOOST_TEST((*it).first == 15);
  ++it;
  BOOST_TEST(it == tree.end());
}

BOOST_AUTO_TEST_CASE(drop_test)
{
  losev::BSTree<int, std::string> tree;
  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");

  std::string val = tree.drop(5);
  BOOST_TEST(val == "five");
  BOOST_TEST(tree.size() == 2);
  bool thrown = false;
  try { tree.get(5); } catch (const std::out_of_range&) { thrown = true; }
  BOOST_TEST(thrown == true);
}

BOOST_AUTO_TEST_CASE(drop_root_test)
{
  losev::BSTree<int, std::string> tree;
  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");

  std::string val = tree.drop(10);
  BOOST_TEST(val == "ten");
  BOOST_TEST(tree.size() == 2);
  bool thrown = false;
  try { tree.get(10); } catch (const std::out_of_range&) { thrown = true; }
  BOOST_TEST(thrown == true);
}

BOOST_AUTO_TEST_CASE(height_test)
{
  losev::BSTree<int, std::string> tree;
  BOOST_TEST(tree.height() == 0);

  tree.push(10, "ten");
  BOOST_TEST(tree.height() == 1);

  tree.push(5, "five");
  BOOST_TEST(tree.height() == 2);

  tree.push(15, "fifteen");
  BOOST_TEST(tree.height() == 2);

  tree.push(3, "three");
  BOOST_TEST(tree.height() == 3);

  tree.push(1, "one");
  BOOST_TEST(tree.height() == 4);
}

BOOST_AUTO_TEST_CASE(rotate_left_test)
{
  losev::BSTree<int, std::string> tree;
  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");
  tree.push(12, "twelve");
  tree.push(18, "eighteen");

  auto it = tree.begin();
  while ((*it).first != 10) ++it;
  auto newIt = tree.rotateLeft(it);
  BOOST_TEST((*newIt).first == 15);
}

BOOST_AUTO_TEST_CASE(rotate_right_test)
{
  losev::BSTree<int, std::string> tree;
  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");
  tree.push(3, "three");
  tree.push(7, "seven");

  auto it = tree.begin();
  while ((*it).first != 10) ++it;
  auto newIt = tree.rotateRight(it);
  BOOST_TEST((*newIt).first == 5);
}

BOOST_AUTO_TEST_CASE(rotate_large_left_test)
{
  losev::BSTree<int, std::string> tree;
  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");
  tree.push(12, "twelve");
  tree.push(13, "thirteen");

  auto it = tree.begin();
  while ((*it).first != 10) ++it;
  auto newIt = tree.rotateLargeLeft(it);
  BOOST_TEST((*newIt).first == 12);
}

BOOST_AUTO_TEST_CASE(rotate_large_right_test)
{
  losev::BSTree<int, std::string> tree;
  tree.push(10, "ten");
  tree.push(15, "fifteen");
  tree.push(5, "five");
  tree.push(7, "seven");
  tree.push(6, "six");

  auto it = tree.begin();
  while ((*it).first != 10) ++it;
  auto newIt = tree.rotateLargeRight(it);
  BOOST_TEST((*newIt).first == 7);
}

BOOST_AUTO_TEST_SUITE_END()
