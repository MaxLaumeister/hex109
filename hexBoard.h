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

inline bool hexBoard::isValidSpace(int index) const {
    return index >= 0 && index < spaces->size();
}

inline bool hexBoard::isValidSpace(int x, int y) const {
    return x >= 0 && y >= 0 && x < sideLength && y < sideLength;
}

inline bool hexBoard::isValidMove(int x, int y, bool piRule) const {
    return isValidSpace(x, y) && (getSpace(x, y) == P_EMPTY || piRule);
}

inline bool hexBoard::isValidMove(int index, bool piRule) const {
    return isValidSpace(index) && (getSpace(index) == P_EMPTY || piRule);
}

inline int hexBoard::getIndex(int x, int y) const {
    return sideLength*y + x;
}

inline pair<int, int> hexBoard::getCoords(int index) const {
    int x = index % sideLength;
    int y = index / sideLength;
    return pair<int, int>(x, y);
}

inline Space hexBoard::getSpace(int index) const {
    // assert(isValidSpace(index));
    return (*spaces)[index];
}

inline Space hexBoard::getSpace(int x, int y) const {
    return getSpace(getIndex(x, y));
}

inline void hexBoard::setSpace(int index, Space sp) {
    // assert(isValidSpace(index));
    (*spaces)[index] = sp;
}

inline void hexBoard::setSpace(int x, int y, Space sp) {
    setSpace(getIndex(x, y), sp);
}



#endif	/* HEXBOARD_H */

