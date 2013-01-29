#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_NO_MAIN

#include <boost/test/unit_test.hpp>
#include "../GTPEngine.h"
#include "../Exception.h"

BOOST_AUTO_TEST_SUITE(test)
BOOST_AUTO_TEST_CASE(test_test) {
	GTPEngine g;

  //Col string to int
  BOOST_CHECK_EQUAL(g.ColumnStringToInt("A"),0);
  BOOST_CHECK_THROW(g.ColumnStringToInt("I"), Exception);
  BOOST_CHECK_THROW(g.ColumnStringToInt("I"), Exception);
  BOOST_CHECK_NO_THROW(g.ColumnStringToInt("a"));
  BOOST_CHECK_THROW(g.ColumnStringToInt("1"), Exception);
  BOOST_CHECK_NO_THROW(g.ColumnStringToInt("A"));

  //Row string to int
  BOOST_CHECK_EQUAL(g.RowStringToInt("1"),0);
  BOOST_CHECK_EQUAL(g.RowStringToInt("19"),18);
  BOOST_CHECK_THROW(g.RowStringToInt("20"), Exception);
  BOOST_CHECK_THROW(g.RowStringToInt("a"), Exception);
  BOOST_CHECK_THROW(g.RowStringToInt("A"), Exception);
  
}

BOOST_AUTO_TEST_SUITE_END();