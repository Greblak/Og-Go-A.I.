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
	boost::asio::deadline_timer simulationTimer; ///< Timer to halt simulations based on time settings
	AiType aiType; ///< Current AI type
	int timeAlloc; ///< Number of seconds available for simulations
	int simulations; ///< Number of simulations that will be run
	std::vector<int> preselRandMoves; ///< A container of moves to be evaluated
	int numRandMoves; ///< Number of random moves added

};



#endif /* EGTPENGINE_H_ */
