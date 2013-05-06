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
#include "MonteCarlo.h"


void TEST_PlayPolicy();
void TEST_GoBoard_Methods();
void TEST_BoardRepresentation_LibertyCounting();
void TEST_SimpleRandomAI();
void TEST_EGTPEngine();
void TEST_DOTESTS()
{
  std::cout<<"Testing board representation"<<std::endl;
  std::cout<<"** GoBoard member methods"<<std::endl;
  TEST_GoBoard_Methods();
  std::cout<<"** Liberty counting"<<std::endl;
  TEST_BoardRepresentation_LibertyCounting();
  std::cout<<"Testing GTP Engines"<<std::endl;
  std::cout<<"** EGTP Engine"<<std::endl;
  TEST_EGTPEngine();
  std::cout<<"Testing A.I.s"<<std::endl;
  std::cout<<"** Single randomly generated legal move(100 000 games)"<<std::endl;
  TEST_SimpleRandomAI();  
  std::cout<<"All tests completed successfully"<<std::endl;
}
  void TEST_GoBoard_Methods()
  {

    //Testing simple startup and some commands
    GoBoard* b = new GoBoard(19);
    assert(b->Size() == 19);
    assert(!b->Occupied(0)); //Not played at
    b->Play(0,0);
    assert(b->Occupied(0)); //Now played at
    assert(b->IsCorner(0)); // Bottom left corner
    assert(b->Liberties(0) == 2); // Corner has 2 libs
    b->Play(1,0);
    assert(!b->IsCorner(1)); // Right of bottom left corner
    assert(b->IsBorder(1)); // Bottom border
    assert(b->Liberties(0) == 3); // Group should have 3 libs
    assert(b->CurrentPlayer() == S_BLACK); 
    assert(b->CurrentPlayer() != b->NextPlayer());
    assert(b->South(0) == -1); //Point does not exist
    assert(b->West(0) == -1); //Point does not exist
    assert(b->East(0) == 1); 
    assert(b->East(18) == -1); //Point does not exist.
    assert(b->North(0) == 19); 
    assert(b->IsRealPoint(0));
    assert(b->IsRealPoint(360)); // Last real point
    assert(!b->IsRealPoint(361));
    assert(b->IsRealPoint(19)); 
    assert(b->IsLegal(2,S_BLACK)); // Empty point
    assert(!b->IsLegal(0,S_BLACK)); // Occupied point. 

    //Blocktesting
    assert(b->IsInSameBlock(0,1));
    assert(!b->IsInSameBlock(0,2));
    assert(!b->IsInSameBlock(2,1));
    assert(!b->IsInSameBlock(2,3));
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
  //Testing 100 000 random games

  SimpleRandomAI rand;
  int color = S_BLACK;
  GoGame g(19);
  for(int j = 0; j<100000;++j)
    {
      if(j%1000==0)
	std::cout<<j<<" games"<<std::endl;
      g.Board->reset();
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
