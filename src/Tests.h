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
  std::cout<<"Size()"<<std::endl;
GoBoard* b = new GoBoard(9);
  assert(b->Size() == 9);
  delete b;

  b = new GoBoard(13);
  assert(b->Size() == 13);
  delete b;

  b = new GoBoard(19);
  assert(b->Size() == 19);

b = new GoBoard(9);
  assert(b->Size() == 9);

  //Occupied
  std::cout<<"Occupied()"<<std::endl;
  assert(!b->Occupied(0)); //Not played at
  b->Play(0,0);
  assert(b->Occupied(0)); //Now played at
  //Check stone removed
  b->Play(9,1);
  b->Play(1,1);
  assert(!b->Occupied(0)); //Now captured

  std::cout<<"IsCorner()"<<std::endl;
  assert(b->IsCorner(0)); // Bottom left corner
  assert(!b->IsCorner(1)); // Right of bottom left corner
  assert(!b->IsCorner(-1)); // invalid
  assert(!b->IsCorner(22)); // Valid, center
  std::cout<<"IsBorder()"<<std::endl;
  assert(b->IsBorder(0)); // bottom left corner
  assert(b->IsBorder(1)); // Bottom border
  assert(!b->IsBorder(-1)); // Invalid
  assert(!b->IsBorder(22)); // Not border
  
  std::cout<<"CurrentPlayer() "<<std::endl;
  b->Play(5,1);
  assert(b->CurrentPlayer() == S_BLACK); //Black after white
  assert(b->CurrentPlayer() != b->NextPlayer());
  b->Play(6,0);
  assert(b->CurrentPlayer() == S_WHITE); 

  std::cout<<"Directional tests"<<std::endl;
  std::cout<<"North()"<<std::endl;
  //North tests
  assert(b->North(-1) == -1); 
  assert(b->North(0) == 9); 
  assert(b->North(8) == 17);
  assert(b->North(72) == -1);
  assert(b->North(80) == -1);
  assert(b->North(81) == -1);
  

  std::cout<<"South()"<<std::endl;
  assert(b->South(-1) == -1);
  assert(b->South(0) == -1);
  assert(b->South(8) == -1);
  assert(b->South(72) == 63); 
  assert(b->South(80) == 71); 
  assert(b->South(81) == -1); 

  std::cout<<"West()"<<std::endl;
  assert(b->West(-1) == -1);
  assert(b->West(0) == -1); 
  assert(b->West(8) == 7); 
  assert(b->West(72) == -1);
  assert(b->West(80) == 79);
  assert(b->West(81) == -1);

  std::cout<<"East()"<<std::endl;
  assert(b->East(-1) == -1);
  assert(b->East(0) == 1); 
  assert(b->East(8) == -1);
  assert(b->East(72) == 73);
  assert(b->East(80) == -1);
  assert(b->East(81) == -1); 
  std::cout<<"End of directional tests"<<std::endl;

  std::cout<<"IsRealPoint()"<<std::endl;
  assert(b->IsRealPoint(0));
  assert(b->IsRealPoint(80)); // Last real point
  assert(!b->IsRealPoint(81));


}
void TEST_BoardRepresentation_LibertyCounting()
{
  std::cout<<"***Position A"<<std::endl;
  {
    GoGame g(9);
    g.Play(S_BLACK,0,0);
    g.Play(S_BLACK,1,0);
    g.Play(S_BLACK,2,0);
    g.Play(S_BLACK,3,0);
    GoBlock* p_block = g.Board->State.blockPointers[g.Board->Pos(GoPoint(0,0,S_BLACK))];
    assert(p_block->Liberties() == 5);
  }

  std::cout<<"***Position B"<<std::endl;
  {
    GoGame g(9);
    g.Play(S_BLACK,0,1);
    g.Play(S_BLACK,1,1);
    g.Play(S_BLACK,2,1);
    g.Play(S_BLACK,3,1);
    GoBlock* p_block = g.Board->State.blockPointers[g.Board->Pos(GoPoint(0,1,S_BLACK))];
    assert(p_block->Liberties() == 9);
  }

  std::cout<<"***Position C"<<std::endl;
  {
    GoGame g = GoGame(9);
    g.Play(S_BLACK,1,1);
    g.Play(S_BLACK,2,1);
    g.Play(S_BLACK,3,1);
    g.Play(S_BLACK,4,1);
    GoBlock* p_block = g.Board->State.blockPointers[g.Board->Pos(GoPoint(1,1,S_BLACK))];
    assert(p_block->Liberties() == 10);
  }

  std::cout<<"***Position D"<<std::endl;
  {
    GoGame g = GoGame(9);
    g.Play(S_BLACK,1,1);
    g.Play(S_BLACK,2,1);
    g.Play(S_BLACK,2,2);
    g.Play(S_BLACK,2,3);
    GoBlock* p_block = g.Board->State.blockPointers[g.Board->Pos(GoPoint(1,1,S_BLACK))];
    assert(p_block->Liberties() == 9);
  }
  std::cout<<"***Position E"<<std::endl;
  {
    GoGame g = GoGame(9);
    g.Play(S_BLACK,1,1);
    g.Play(S_BLACK,2,1);
    g.Play(S_BLACK,2,2);
    g.Play(S_BLACK,3,2);
    g.Play(S_BLACK,2,3);
    GoBlock* p_block = g.Board->State.blockPointers[g.Board->Pos(GoPoint(1,1,S_BLACK))];
    assert(p_block->Liberties() == 9);
  }
  std::cout<<"***Position F"<<std::endl;
  {
    GoGame g = GoGame(9);
    g.Play(S_BLACK,1,1);
    g.Play(S_BLACK,2,1);
    g.Play(S_BLACK,2,2);
    g.Play(S_BLACK,3,2);
    g.Play(S_BLACK,1,2);
    g.Play(S_BLACK,2,3);
    GoBlock* p_block = g.Board->State.blockPointers[g.Board->Pos(GoPoint(1,1,S_BLACK))];
    assert(p_block->Liberties() == 9);
  }

  std::cout<<"***Position G"<<std::endl;
  {
    GoGame g = GoGame(9);
    g.Play(S_BLACK,3,1);
    g.Play(S_BLACK,2,1);
    g.Play(S_BLACK,2,3);
    g.Play(S_BLACK,2,2);
    g.Play(S_BLACK,4,2);
    g.Play(S_BLACK,4,3);
    g.Play(S_BLACK,4,1);
    GoBlock* p_block = g.Board->State.blockPointers[g.Board->Pos(GoPoint(3,1,S_BLACK))];
    assert(p_block->Liberties() == 13);
  }
 std::cout<<"***Position H"<<std::endl;
  {
    GoGame g = GoGame(9);
    g.Play(S_BLACK,3,1);
    g.Play(S_WHITE,3,2);
    g.Play(S_BLACK,3,3);
    g.Play(S_WHITE,4,2);
    g.Play(S_BLACK,2,1);
    g.Play(S_WHITE,3,4);
    g.Play(S_BLACK,2,3);
    g.Play(S_WHITE,6,4);
    g.Play(S_BLACK,2,2);
    g.Play(S_WHITE,4,1);
    g.Play(S_BLACK,4,3);
    g.Play(S_BLACK,5,2);
    g.Play(S_WHITE,5,4);
    g.Play(S_BLACK,5,1);
    g.Play(S_WHITE,2,4);
    g.Play(S_BLACK,2,5);
    g.Play(S_BLACK,5,3);
    g.Play(S_WHITE,7,0);
    g.Play(S_BLACK,5,0);
    g.Play(S_WHITE,6,0);
    g.Play(S_BLACK,4,0);
    GoBlock* p_block = g.Board->State.blockPointers[g.Board->Pos(GoPoint(3,1,S_BLACK))];
    assert(p_block->Liberties() == 12);
  }
  std::cout<<"***Position I"<<std::endl;
  {
    GoGame g = GoGame(9);
    g.Play(S_BLACK,3,1);
    g.Play(S_WHITE,3,2);
    g.Play(S_BLACK,3,3);
    g.Play(S_WHITE,0,0);
    g.Play(S_BLACK,2,1);
    g.Play(S_WHITE,3,4);
    g.Play(S_BLACK,2,3);
    g.Play(S_WHITE,6,4);
    g.Play(S_BLACK,2,2);
    g.Play(S_WHITE,4,1);
    g.Play(S_BLACK,4,3);
    g.Play(S_BLACK,5,2);
    g.Play(S_WHITE,5,4);
    g.Play(S_BLACK,5,1);
    g.Play(S_WHITE,2,4);
    g.Play(S_BLACK,2,5);
    g.Play(S_BLACK,5,3);
    g.Play(S_WHITE,7,0);
    g.Play(S_BLACK,5,0);
    g.Play(S_WHITE,6,0);
    g.Play(S_BLACK,4,0);
    g.Play(S_WHITE,0,1);
    g.Play(S_BLACK,4,2);
    GoBlock* p_block = g.Board->State.blockPointers[g.Board->Pos(GoPoint(3,1,S_BLACK))];
    assert(p_block->Liberties() == 11);
  }
 


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
