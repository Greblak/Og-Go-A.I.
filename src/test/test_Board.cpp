#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_NO_MAIN

#include <boost/test/unit_test.hpp>
#include "../GoBoard.h"
#include "../Exception.h"

BOOST_AUTO_TEST_SUITE(Board)
BOOST_AUTO_TEST_CASE(Board_Test) {

  GoBoard* b = new GoBoard(19);
  int c1 = b->Pos(GoPoint(0,0,0));
  int c2 = b->Pos(GoPoint(0,18,0));
  int c3 = b->Pos(GoPoint(18,0,0));
  int c4 = b->Pos(GoPoint(18,18,0));
  

  int b1 = b->Pos(GoPoint(18,18,0));
  int b2 = b->Pos(GoPoint(0,5,0));
  int b3 = b->Pos(GoPoint(18,3,0));
  int b4 = b->Pos(GoPoint(0,0,0));
  int b5 = b->Pos(GoPoint(6,18,0));
  int b6 = b->Pos(GoPoint(8,0,0));
  int nb7 = b->Pos(GoPoint(17,18,0));
  int nb8 = b->Pos(GoPoint(1,1,0));
  

  //Checking corners
  BOOST_CHECK_EQUAL(b->IsCorner(c1), true);
  BOOST_CHECK_EQUAL(b->IsCorner(c2), true);
  BOOST_CHECK_EQUAL(b->IsCorner(c3), true);
  BOOST_CHECK_EQUAL(b->IsCorner(c4), true);
  
  BOOST_CHECK_EQUAL(b->IsBorder(b1), true);
  BOOST_CHECK_EQUAL(b->IsBorder(b2), true);
  BOOST_CHECK_EQUAL(b->IsBorder(b3), true);
  BOOST_CHECK_EQUAL(b->IsBorder(b4), true);
  BOOST_CHECK_EQUAL(b->IsBorder(b6), true);
  BOOST_CHECK_EQUAL(b->IsBorder(nb7), false);
  BOOST_CHECK_EQUAL(b->IsBorder(nb8), false);
}

BOOST_AUTO_TEST_SUITE_END();