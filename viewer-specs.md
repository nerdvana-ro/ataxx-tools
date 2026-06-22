Hello. I would like your assistance in writing a browser-based viewer for the game Ataxx.

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
