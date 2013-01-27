#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_NO_MAIN

#include <boost/test/unit_test.hpp>
#include "../GTPEngine.h"
#include "../Exception.h"

BOOST_AUTO_TEST_SUITE(test)
BOOST_AUTO_TEST_CASE(test_test) {
	GTPEngine g;

  //Col string to int
  BOOST_CHECK_EQUAL(g.ColumnStringToInt("A1"),0);
  BOOST_CHECK_THROW(g.ColumnStringToInt("I20"), Exception);
  BOOST_CHECK_THROW(g.ColumnStringToInt("I2"), Exception);
  BOOST_CHECK_THROW(g.ColumnStringToInt("a"), Exception);
  BOOST_CHECK_THROW(g.ColumnStringToInt("11"), Exception);
  BOOST_CHECK_NO_THROW(g.ColumnStringToInt("AA"));

  //Row string to int
  BOOST_CHECK_EQUAL(g.RowStringToInt("A1"),0);
  BOOST_CHECK_EQUAL(g.RowStringToInt("A19"),18);
  BOOST_CHECK_THROW(g.RowStringToInt("A20"), Exception);
  BOOST_CHECK_THROW(g.RowStringToInt("aa"), Exception);
  BOOST_CHECK_THROW(g.RowStringToInt("AA"), Exception);
  
}

BOOST_AUTO_TEST_SUITE_END();