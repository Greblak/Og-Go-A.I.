/*
 * SimpleRandomAI.h
 *
 *  Created on: Mar 10, 2013
 *      Author: rune
 */

#ifndef SIMPLERANDOMAI_H_
#define SIMPLERANDOMAI_H_

#include "GoPoint.h"
#include "GoGame.h"

class SimpleRandomAI
{
public:
  SimpleRandomAI();
  virtual
  ~SimpleRandomAI();

  GoPoint generateMove(const int color, GoGame* game);
};

#endif /* SIMPLERANDOMAI_H_ */
