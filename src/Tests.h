/*
 * Tests.h
 *
 *  Created on: Mar 16, 2013
 *      Author: rune
 */

#ifndef TESTS_H_
#define TESTS_H_

#include "EGTPEngine.h"
#include "SimpleRandomAI.h"


void TEST_PlayPolicy();
void TEST_BoardRepresentation_LibertyCounting();
void TEST_SimpleRandomAI();
void TEST_EGTPEngine();

void TEST_DOTESTS()
{
  std::cout<<"Testing board representation"<<std::endl;
  std::cout<<"** Liberty counting"<<std::endl;
  TEST_BoardRepresentation_LibertyCounting();
  std::cout<<"Testing GTP Engines"<<std::endl;
  std::cout<<"** EGTP Engine"<<std::endl;
  TEST_EGTPEngine();
  std::cout<<"Testing A.I.s"<<std::endl;
  std::cout<<"** Single randomly generated legal move"<<std::endl;
  TEST_SimpleRandomAI();
  std::cout<<"All tests completed successfully"<<std::endl;
}

void TEST_BoardRepresentation_LibertyCounting()
{
  //Testing semi-complicated counting position
  GoGame g(9);
  g.Play(S_BLACK,1,1);
  g.Play(S_BLACK,3,1);
  g.Play(S_BLACK,2,1);
  g.Play(S_BLACK,1,2);
  g.Play(S_WHITE,2,2);
  g.Play(S_BLACK,3,2);
  g.Play(S_BLACK,2,3);
  GoBlock* p_block = g.Board->State.blockPointers[g.Board->Pos(GoPoint(1,1,S_BLACK))];
  assert(p_block->Liberties() == 10);

  //Testing replay functionality
  GoGame gg(9);
  gg.Board->resetAndReplayMoves(g.Board);
  GoBlock* p_block1 = gg.Board->State.blockPointers[gg.Board->Pos(GoPoint(1,1,S_BLACK))];
  assert(p_block1->Liberties() == p_block->Liberties());
  assert(gg.Board->State.stones[gg.Board->Pos(GoPoint(2,2,NONE))] == NONE);
}
void TEST_SimpleRandomAI()
{
  //Testing 50 random games

  SimpleRandomAI rand;
  int color = S_BLACK;
  for(int j = 0; j<50;++j)
    {
      GoGame g(19);
      for(int i = 0; i<361; ++i)
	{
	  color = color == S_BLACK ? S_WHITE : S_BLACK;
	  const GoPoint p = rand.generateMove(color,&g);
	  if(!p.IsPass() && g.Board->IsLegal(p,p.color))
	    g.Play(p.color,p.x,p.y);
	}
    }
};

void TEST_EGTPEngine()
{
  /*

    Currently invalidated. Requires io_service
    EGTPEngine gtp;
    std::string input[3] = {"boardsize 9", "e_useai ucb s 10", "genmove b"};
    std::vector<std::string> cmd;

    for(int i = 0; i < 3; ++i)
    {
    gtp.parse(input[i]);
    }

  */
};

#endif /* TESTS_H_ */
