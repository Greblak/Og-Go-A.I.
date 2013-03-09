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
  /*
   * Generates an SGF-file based on the moves played in the given game
   */
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
    //End of premable. LET'S PLAY!

    for (GoMoveIterator it = game->moves.begin(); it != game->moves.end(); ++it)
      {
        char x = GTPEngine::ColumnIntToString((*it)->Point.x);
        x = tolower(x);
        int i_y = game->Board->Size() - ((*it)->Point.y) - 1; //Reverses due to odd notation in SGF
        char y = GTPEngine::ColumnIntToString(i_y);
        y = tolower(y);
        y = y > 'i' ? y-1 : y;
        x = x > 'i' ? x-1 : x;

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
