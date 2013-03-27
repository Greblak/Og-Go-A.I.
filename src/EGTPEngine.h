/*
 * EGTPEngine.h
 *
 *  Created on: Mar 27, 2013
 *      Author: rune
 */

#ifndef EGTPENGINE_H_
#define EGTPENGINE_H_

#include "GTPEngine.h"
enum AiType{
	RAN = 0,
	MC,
	UCB,
	UCT
};
class EGTPEngine : public GTPEngine
{
public:
	EGTPEngine();
	virtual ~EGTPEngine();

	std::vector<std::string> parse(std::string input);
private:
	AiType aiType;
	int timeAlloc;
	int simulations;
	int randMoves;
};



#endif /* EGTPENGINE_H_ */
