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
    generateFile(game->Board);
  }

  static inline void generateFile(const GoBoard* Board)
  {
    LOG_VERBOSE << "Attempting to generate SGF file";
    std::ofstream file;
    file.open("SGF.sgf");
    if(file.fail())
      {
        LOG_DEBUG << "SGF file not found. No SGF data generated";
        return;
      }
    else
      LOG_VERBOSE << "Generating SGF";
    //Write preamble
    file << "(" << std::endl;
    file << ";FF[4]GM[1]" << std::endl;
    file << "PB[Player 1]" << std::endl;
    file << "PW[Player 2]" << std::endl;
    file << "RU[Japanese]" << std::endl;
    file << "KM["<<Board->komi<<"]" << std::endl;
    file << "SZ["<<Board->Size()<<"]" << std::endl;
    //End of premable. LET'S PLAY!

    for(int i = 0; i<Board->movePointer;++i)
    {
        char x = GTPEngine::ColumnIntToString(Board->moves[i]->Point.x);
        x = tolower(x);
        int i_y = Board->Size() - (Board->moves[i]->Point.y) - 1; //Reverses due to odd notation in SGF
        char y = GTPEngine::ColumnIntToString(i_y);
        y = tolower(y);
        y = y > 'i' ? y-1 : y;
        x = x > 'i' ? x-1 : x;

        char col = Board->moves[i]->Color == S_BLACK ? 'B' : 'W';
        file << ";"<<col<<"["<<x<<y<<"]" <<std::endl;
        char separator = Board->moves[i] == Board->moves[0] ? ' ' : ';';
        LOG_VERBOSE <<"Output to SGF:" << separator <<col<<"["<<x<<y<<"]" <<std::endl;
      }

    //End file
    file << ")" <<std::endl;
  }
};

#endif //#ifndef __SGFENGINE_H__
