#include "Board.h"
#include "Config.h"
#include "Log.h"
#include "Move.h"
#include "RandomAgent.h"

int main(int argc, char** argv) {
  Board::precomputeJumpDomains();

  Board board;
  board.readFromStdin();
  board.print();

  int time[2];
  scanf("%d %d", &time[0], &time[1]);
  int my_time = time[board.side];
  Log::debug("Time: %d milliseconds for an estimated %d moves.\n",
             my_time, board.estimateRemainingMoves());

  RandomAgent agent(&board);
  Move m = agent.getMove();
  printf("%s\n", m.toString().c_str());

  return 0;
}
