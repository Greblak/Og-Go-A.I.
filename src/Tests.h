/*
 * Tests.h
 *
 *  Created on: Mar 16, 2013
 *      Author: rune
 */

#ifndef TESTS_H_
#define TESTS_H_


void TEST_PlayPolicy();
void TEST_UpperConfidence();

void TEST_UpperConfidence()
{
  GoGame g(9);
  g.Play(S_BLACK,1,1);
  g.Play(S_BLACK,3,1);
  g.Play(S_WHITE,2,1);
//
//  GoGame gg(9);
//  gg.Board->resetAndReplayMoves(g.Board);

}

#endif /* TESTS_H_ */
