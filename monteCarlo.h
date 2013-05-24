#ifndef MONTE_CARLO_H
#define	MONTE_CARLO_H

#include "game.h"

namespace monteCarlo {
    void getMove(const hexBoard* board, int iterations, Agent thisMove = COM, Agent lastMove = PLAYER);
}

#endif	/* MONTE_CARLO_H */

