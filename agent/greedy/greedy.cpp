// Același agent ca și Doofus, dar condensat într-un singur fișier. Fără
// logging. Alege mereu mutarea cea mai profitabilă.
#include <sstream>
#include <stdio.h>
#include <string>

typedef unsigned char u8;
typedef unsigned long long u64;

// Ține-le sincronizate.
const int BOARD_SIZE = 7;
const u64 LEFT_COL =   0b0000001'0000001'0000001'0000001'0000001'0000001'0000001ll;
const u64 RIGHT_COL =  0b1000000'1000000'1000000'1000000'1000000'1000000'1000000ll;
const u64 TOP_ROW =    0b0000000'0000000'0000000'0000000'0000000'0000000'1111111ll;
const u64 BOTTOM_ROW = 0b1111111'0000000'0000000'0000000'0000000'0000000'0000000ll;

const int MAX_MOVES = 200;

const int M_CLONE = 1;
const int M_JUMP = 2;

const int CLONE_DELTA[8][2] = {
  {-1, -1}, {-1, 0}, {-1, +1},
  {0, -1}, {0, +1},
  {+1, -1}, {+1, 0}, {+1, +1},
};

const int JUMP_DELTA[16][2] = {
  {-2, -2}, {-2, -1}, {-2, 0}, {-2, +1}, {-2, +2},
  {-1, -2}, {-1, +2},
  {0, -2}, {0, +2},
  {+1, -2}, {+1, +2},
  {+2, -2}, {+2, -1}, {+2, 0}, {+2, +1}, {+2, +2},
};

class Util {
public:
  static std::string squareName(int sq) {
    std::string s;
    s = ('a' + sq % BOARD_SIZE);
    s += ('1' + sq / BOARD_SIZE);
    return s;
  }
};


class Move {
public:
  u8 type;
  u8 src, dest;

  std::string toString() {
    return (type == M_CLONE)
      ? Util::squareName(dest)
      : Util::squareName(src) + '-' + Util::squareName(dest);
  }
};


class Board {
public:
  // Pentru fiecare pătrat, precalculează pătratele la distanță 1 și 2.
  static u64 cloneDomains[BOARD_SIZE * BOARD_SIZE];
  static u64 jumpDomains[BOARD_SIZE * BOARD_SIZE];

  u64 pieces[2];
  u64 empty;
  bool side;

  static void precomputeDomains() {
    precomputeTypeDomains(CLONE_DELTA, 8, Board::cloneDomains);
    precomputeTypeDomains(JUMP_DELTA, 16, Board::jumpDomains);
  }

  static void precomputeTypeDomains(const int (*delta)[2], int count, u64* dest) {
    for (int r = 0; r < BOARD_SIZE; r++) {
      for (int c = 0; c < BOARD_SIZE; c++) {
        int bit = r * BOARD_SIZE + c;
        for (int i = 0; i < count; i++) {
          int r2 = r + delta[i][0];
          int c2 = c + delta[i][1];
          if ((r2 >= 0) && (r2 < BOARD_SIZE) &&
              (c2 >= 0) && (c2 < BOARD_SIZE)) {
            dest[bit] |= 1ll << (r2 * BOARD_SIZE + c2);
          }
        }
      }
    }
  }

  void readFromStdin() {
    int x;
    scanf("%d ", &x);
    side = x;

    pieces[0] = pieces[1] = empty = 0;

    for (int r = 0; r < BOARD_SIZE; r++) {
      for (int c = 0; c < BOARD_SIZE; c++) {
        u64 mask = 1ll << (r * BOARD_SIZE + c);
        char ch = getchar();
        switch (ch) {
          case 'x': pieces[0] |= mask; break;
          case 'o': pieces[1] |= mask; break;
          case '.': empty |= mask; break;
        }
      }
      getchar(); // '\n'
    }
  }

  int eval() {
    return __builtin_popcountll(pieces[side]) - __builtin_popcountll(pieces[!side]);
  }

  void makeMove(Move m) {
    if (m.type == M_JUMP) {
      // Curăță sursa.
      pieces[side] ^= 1ll << m.src;
      empty ^= 1ll << m.src;
    }

    // Umple destinația.
    pieces[side] ^= 1ll << m.dest;
    empty ^= 1ll << m.dest;

    // Întoarce vecinii.
    u64 opp_neighbors = pieces[!side] & cloneDomains[m.dest];
    pieces[side] ^= opp_neighbors;
    pieces[!side] ^= opp_neighbors;

    side = !side;
  }
};


class MoveGen {
public:
  Board* board;
  Move* moves;
  int numMoves;

  MoveGen(Board* board, Move* moves) {
    this->board = board;
    this->moves = moves;
    numMoves = 0;
  }

  void run() {
    genClones();
    genJumps();
  }

private:
  void genClones() {
    u64 mask = board->pieces[board->side];
    mask |= (mask & ~LEFT_COL) >> 1;
    mask |= (mask & ~RIGHT_COL) << 1;
    mask |= (mask & ~TOP_ROW) >> BOARD_SIZE;
    mask |= (mask & ~BOTTOM_ROW) << BOARD_SIZE;
    mask &= board->empty;
    while (mask) {
      int bit = __builtin_ctzll(mask);
      mask ^= 1ll << bit;
      pushMove(M_CLONE, 0, bit);
    }
  }

  void genJumps() {
    u64 src = board->pieces[board->side];
    while (src) {
      int sbit = __builtin_ctzll(src);
      src ^= 1ll << sbit;
      u64 dest = Board::jumpDomains[sbit] & board->empty;
      while (dest) {
        int dbit = __builtin_ctzll(dest);
        dest ^= 1ll << dbit;
        pushMove(M_JUMP, sbit, dbit);
      }
    }
  }

  void pushMove(u8 type, u8 src, u8 dest) {
    Move& m = moves[numMoves++];
    m.type = type;
    m.src = src;
    m.dest = dest;
  }
};


class GreedyAgent {
public:
  Board* board;
  Move moves[MAX_MOVES];

  GreedyAgent(Board* board) {
    this->board = board;
  }

  Move getMove() {
    MoveGen moveGen(board, moves);
    moveGen.run();

    int bestMove;
    int bestScore = BOARD_SIZE * BOARD_SIZE + 1;

    for (int i = 0; i < moveGen.numMoves; i++) {
      Board b2 = *board;
      b2.makeMove(moves[i]);
      int e = b2.eval();
      if (e < bestScore) { // Căutăm cea mai proastă poziție pentru adversar.
        bestScore = e;
        bestMove = i;
      }
    }

    return moves[bestMove];
  }
};

u64 Board::cloneDomains[BOARD_SIZE * BOARD_SIZE];
u64 Board::jumpDomains[BOARD_SIZE * BOARD_SIZE];

int main(int argc, char** argv) {
  Board::precomputeDomains();

  Board board;
  board.readFromStdin();

  GreedyAgent agent(&board);
  Move m = agent.getMove();
  printf("%s\n", m.toString().c_str());

  return 0;
}
