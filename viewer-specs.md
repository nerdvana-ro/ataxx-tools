Hello. I would like your assistance in writing a browser-based viewer for the game Ataxx.

I have placed all the relevant documents at https://nerdvana.ro/ataxx-viewer/

# The game of Ataxx

* Ataxx is an abstract strategy game for two players.
* It is played on a 7×7 square grid.
* Rows are numbered `1` through `7` from top to bottom.
* Columns are named `a` through `g` from left to right.
* In a physical game there are 49 two-sided pieces, red on one side and blue on the other (similar to Reversi / Othello).
* In the starting position, the first player ("Red") has two red pieces at `a1` and `g7`.
* The other player ("Blue") has two blue pieces at `a7` and `g1`.
* Players take turns moving, with Red going first.
* There are two types of moves. A player may _clone_ a piece by placing a piece of their color on an empty square that is adjacent vertically, horizontally or diagonally to one of their existing pieces.
* A player may _jump_ by moving one of their pieces to an empty square that is two squares away horizontally, vertically or diagonally. For example, in the starting position Red may jump from `a1` to `c1`, `c2`, `a3`, `b3`, or `c3`. To clarify, a player may not jump one square away.
* After the move, all of the opponent player's pieces adjacent to the destination square are converted (flipped) to the color of the moving player.
* The game ends when the board fills up. The player who has more pieces wins. Since the number of squares is odd, there are no ties.

For our implementation, we use the following modification to the original rules.

* In the original rules, if a player may not move, either because he has no pieces left or because none of those pieces can reach an empty square by jumping or cloning, then he must pass. In our rules, if a player has no moves, the game ends and the opponent gets all the empty squares.

# Code organization

This section may or may not be relevant to you.

The subdirectory [ataxx-tools/arbiter](https://nerdvana.ro/ataxx-viewer/ataxx-tools/arbiter/) contains a rudimentary arbiter written in PHP. Its job is to organize round-robin Ataxx tournaments between several agents (computer programs). The arbiter schedules the rounds and runs the games. For each game, the arbiter invokes the two agents alternatively, giving them the current state of the game. Each agent must read the input (the game state), figure out the best move according to its algorithm, print that move and terminate.

The complete protocol is described at [ataxx-tools/protocol.md]([url](https://nerdvana.ro/ataxx-viewer/ataxx-tools/protocol.md)) (in Romanian). Here is a brief summary. Here is a brief summary.

On its turn, each agent receives the following data at the standard input:

* A line containing 0 or 1 to let the agent know if it has the red (0) or the blue (1) pieces.
* Seven lines with 7 characters per line describing the board. `x` denotes a red piece, `o` denotes a blue piece, and `.` denotes an empty square.
* A line containing two integers which denote the remaining clock times, in milliseconds, for Red and Blue respectively.

For example, assuming 3 minutes per person, the initial input data is:

```txt
0
x.....o
.......
.......
.......
.......
.......
o.....x
180000 180000
```

The agent must output a single line at the standard output. The line must contain a move in algebraic notation. For example, on its first move Red may output:

* `b2` to indicate cloning the piece from `a1` at `b2`
* `a1-b3` to indicate jumping from `a1` to `b3`
* etc.

The agent may print any debug information at the standard error. The arbiter will ignore all this **except** lines that begin with the text `kibitz `, which the arbiter collects and writes to the saved game. The agent may kibitz information such as analysis results, forced wins / losses etc.

The subdirectory [andromeda](https://nerdvana.ro/ataxx-viewer/andromeda/) contains a reasonably good agent using alpha-beta with iterative deepening and transposition tables. The subdirectory [ataxx-tools/agent](https://nerdvana.ro/ataxx-viewer/ataxx-tools/agent/) contains more agents which hang / throw errors / print incorrect outputs and so on. I used them to test the robustness of the arbiter.
