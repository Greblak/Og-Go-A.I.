/*
 * Tests.h
 *
 *  Created on: Mar 16, 2013
 *      Author: rune
 */

#ifndef TESTS_H_
#define TESTS_H_

#include "EGTPEngine.h"


void TEST_PlayPolicy();
void TEST_UpperConfidence();

void TEST_UpperConfidence()
{
	GoGame g(9);
	g.Play(S_BLACK,1,1);
	g.Play(S_BLACK,3,1);
	g.Play(S_BLACK,2,1);
	g.Play(S_BLACK,1,2);
	g.Play(S_WHITE,2,2);
	g.Play(S_BLACK,3,2);
	g.Board->DisplayCurrentState();
	g.Play(S_BLACK,2,3);
	g.Board->DisplayCurrentState();
	GoBoard* b = (g.Board);
	GoBlock* p_block = g.Board->State.blockPointers[g.Board->Pos(GoPoint(1,1,S_BLACK))];
//	LOG_DEBUG<<p_block->Liberties();
	//  LOG_DEBUG<<b->State.nu
	//
	//  GoGame gg(9);
	//  gg.Board->resetAndReplayMoves(g.Board);

};

void TEST_EGTPEngine()
{
	EGTPEngine gtp;
	std::string input[3] = {"boardsize 9", "e_useai ucb s 10", "genmove b"};
	std::vector<std::string> cmd;

	for(int i = 0; i < 3; ++i)
	{
		cmd = gtp.parse(input[i]);
	}
};

#endif /* TESTS_H_ */
