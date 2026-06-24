// Același agent ca și Doofus, dar condensat într-un singur fișier. Fără
// logging.
#include <random>
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

class Util {
public:
  static int rand(int lo, int hi) {
    std::uniform_int_distribution<> distrib(lo, hi);
    return distrib(rng);
  }

  static std::string squareName(int sq) {
    std::string s;
    s = ('a' + sq % BOARD_SIZE);
    s += ('1' + sq / BOARD_SIZE);
    return s;
  }

private:
  static std::mt19937 rng;
  static std::random_device rd;
};


class Board {
public:
  // Pentru fiecare pătrat, precalculează pătratele la distanță 2.
  static u64 jumpDomains[BOARD_SIZE * BOARD_SIZE];

  u64 pieces[2];
  u64 empty;
  bool side;

  static void precomputeJumpDomains() {
    const int DELTA[16][2] = {
      {-2, -2}, {-2, -1}, {-2, 0}, {-2, +1}, {-2, +2},
      {-1, -2}, {-1, +2},
      {0, -2}, {0, +2},
      {+1, -2}, {+1, +2},
      {+2, -2}, {+2, -1}, {+2, 0}, {+2, +1}, {+2, +2} };
    for (int r = 0; r < BOARD_SIZE; r++) {
      for (int c = 0; c < BOARD_SIZE; c++) {
        int bit = r * BOARD_SIZE + c;
        for (int i = 0; i < 16; i++) {
          int r2 = r + DELTA[i][0];
          int c2 = c + DELTA[i][1];
          if ((r2 >= 0) && (r2 < BOARD_SIZE) &&
              (c2 >= 0) && (c2 < BOARD_SIZE)) {
            jumpDomains[bit] |= 1ll << (r2 * BOARD_SIZE + c2);
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

  int getNumPiecesToMove() {
    return __builtin_popcountll(pieces[side]);
  }

  int getNumEmpty() {
    return __builtin_popcountll(empty);
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
    if (board->getNumPiecesToMove() < board->getNumEmpty()) {
      genJumpsFromSrc();
    } else {
      genJumpsFromDest();
    }
  }

  // Preferabil cînd numărul de surse (piese) este mic.
  void genJumpsFromSrc() {
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

  // Preferabil cînd numărul de destinații (pătrate goale) este mic.
  void genJumpsFromDest() {
    u64 dest = board->empty;
    while (dest) {
      int dbit = __builtin_ctzll(dest);
      dest ^= 1ll << dbit;
      u64 src = Board::jumpDomains[dbit] & board->pieces[board->side];
      while (src) {
        int sbit = __builtin_ctzll(src);
        src ^= 1ll << sbit;
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


class RandomAgent {
public:
  Board* board;
  Move moves[MAX_MOVES];

  RandomAgent(Board* board) {
    this->board = board;
  }

  Move getMove() {
    MoveGen moveGen(board, moves);
    moveGen.run();
    int i = Util::rand(0, moveGen.numMoves - 1);
    return moveGen.moves[i];
  }
};

std::random_device Util::rd;
std::mt19937 Util::rng(rd());
u64 Board::jumpDomains[BOARD_SIZE * BOARD_SIZE];

int main(int argc, char** argv) {
  Board::precomputeJumpDomains();

  Board board;
  board.readFromStdin();

  RandomAgent agent(&board);
  Move m = agent.getMove();
  printf("%s\n", m.toString().c_str());

  return 0;
}
