/*
 * EGTPEngine.h
 *
 *  Created on: Mar 27, 2013
 *      Author: rune
 */

#ifndef EGTPENGINE_H_
#define EGTPENGINE_H_

#include "GTPEngine.h"
#include "UpperConfidence.h"
#include <boost/asio.hpp>
enum AiType{
	RAN = 0,
	MC,
	UCB,
	UCT
};

const std::string EGTP_HANDSHAKE = "e_request 1";


class EGTPEngine : public GTPEngine
{
public:
	EGTPEngine(boost::asio::io_service& io_service);
	virtual ~EGTPEngine();

	std::string parse(std::string input);
	static GoPoint genMoveFromChildProcs(int color, GoGame* game);
	void genmoveTimer(int seconds, UpperConfidence* ucb);
private:
	boost::asio::deadline_timer simulationTimer;
	AiType aiType;
	int timeAlloc;
	int simulations;
	std::vector<int> preselRandMoves;
	int numRandMoves;

};



#endif /* EGTPENGINE_H_ */
