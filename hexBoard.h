#ifndef HEXBOARD_H
#define	HEXBOARD_H

#include <cstdlib>
#include <vector>

using namespace std;

enum Space {P_EMPTY, P_BLACK, P_WHITE};

class hexBoard {
    public:
      // Constructor adds 4 extra nodes to act as board edge pseudonodes.
      hexBoard(int thisSideLength): sideLength(thisSideLength){
          spaces = new vector<Space>(sideLength * sideLength, P_EMPTY);
      };
      // Copy constructor
      hexBoard(hexBoard const &oldBoard): sideLength(oldBoard.sideLength){
          spaces = new vector<Space>(*(oldBoard.spaces));
      }
      virtual ~hexBoard() {
          delete spaces;
      }

      // Board manipulating functions

      inline int getSize() const {return spaces->size();}
      int getIndex(int x, int y) const;
      pair<int, int> getCoords(int index) const;
      Space getSpace(int index) const;
      Space getSpace(int x, int y) const;
      void setSpace(int index, Space sp);
      void setSpace(int x, int y, Space sp);
      bool isValidMove(int x, int y, bool piRule = false) const;
      bool isValidMove(int index, bool piRule = false) const;
      bool isValidSpace(int index) const;
      bool isValidSpace(int x, int y) const;
      const int sideLength;
    private:
      vector<Space>* spaces;
};

#endif	/* HEXBOARD_H */

