#ifndef __SGFENGINE_H__
#define __SGFENGINE_H__

#include <ctype.h>
#include "GoGame.h"

#include "GoBoard.h"
#include "GTPEngine.h"
#include "GoMove.h"

class SGFEngine
{
 public:
  static inline void generateFile(const GoGame* game)
  {
    std::ofstream file;
    file.open("SGF.sgf");
    //Write preamble
    file << "(" << std::endl;
    file << ";FF[4]GM[1]" << std::endl;
    file << "PB[Player 1]" << std::endl;
    file << "PW[Player 2]" << std::endl;
    file << "RU[Japanese]" << std::endl;
    file << "KM[0.5]" << std::endl;
    file << "SZ["<<game->Board->Size()<<"]" << std::endl;

    for (GoMoveIterator it = game->moves.begin(); it != game->moves.end(); ++it)
      {
	char x = GTPEngine::ColumnIntToString((*it)->Point.x);
	x = tolower(x);
	int i_y = game->Board->Size() - ((*it)->Point.y) - 1; //Reverses due to odd notarion in SGF
	char y = GTPEngine::ColumnIntToString(i_y);
	y = tolower(y);
	y = y > 'i' ? y-1 : y;
	x = x > 'i' ? x-1 : x;
	LOG_DEBUG << "X: "<<(*it)->Point.x<<" " << x;
	LOG_DEBUG << "Y: "<<(*it)->Point.y<<" " << y;
	LOG_DEBUG << "Color: "<<(*it)->Color;

	char col = (*it)->Color == S_BLACK ? 'B' : 'W';
    file << ";"<<col<<"["<<x<<y<<"]" <<std::endl;
    char separator = it == game->moves.begin() ? ' ' : ';';
LOG_DEBUG <<"Output to SGF:" << separator <<col<<"["<<x<<y<<"]" <<std::endl;
      }

					      //End file
    file << ")" <<std::endl;
  }
};

#endif //#ifndef __SGFENGINE_H__
