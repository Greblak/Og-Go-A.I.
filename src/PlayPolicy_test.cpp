/*
 * PlayPolicy_test.cpp
 *
 *  Created on: Mar 16, 2013
 *      Author: rune
 */

#include <iostream>
#include "PlayPolicy.h"
#include "UpperConfidence.h"
#include "GoGame.h"
#include "GoBoard.h"
#include "GoPoint.h"

void TEST_PlayPolicy()
{

  //Testing standard Hane 1
  GoGame g(9);
  g.Play(S_BLACK,1,1);
  g.Play(S_BLACK,3,1);
  g.Play(S_WHITE,2,1);

  PlayPolicy* pp = new PlayPolicy();
  bool match = pp->MatchAny(g.Board,g.Board->Pos(GoPoint(2,2,NONE)), S_BLACK);
  std::cout << "PlayPolicy match #1: " <<match<<std::endl;
  delete pp;


  //Testing CW rotated Hane 1
  GoGame g1(9);
  g1.Play(S_BLACK,3,3);
  g1.Play(S_BLACK,3,1);
  g1.Play(S_WHITE,3,2);

  pp = new PlayPolicy();
  match = pp->MatchAny(g1.Board,g1.Board->Pos(GoPoint(2,2,NONE)), S_BLACK);
  std::cout << "PlayPolicy match #2: " <<match<<std::endl;
}
