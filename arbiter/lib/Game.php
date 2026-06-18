<?php

class Game {
  private array $players;
  private Board $board;
  private bool $badMove;

  private GameInfo $gameInfo;

  function __construct(Player $p1, Player $p2) {
    Log::notice("Partida: {$p1->name} - {$p2->name}");
    $this->players = [$p1, $p2];
    $this->board = new Board();
    $this->badMove = false;
    $this->gameInfo = new GameInfo();
  }

  function run(): void {
    $this->players[0]->startGame();
    $this->players[1]->startGame();

    while (!$this->badMove && $this->board->anyMoves()) {
      $this->playMove();
    }

    $this->computeScores();
  }

  private function playMove(): void {
    Log::info("Mutarea %s", [$this->gameInfo->getNumTurns()]);
    $this->print();
    $input = $this->asInputFile();
    $this->gameInfo->addInput($input);
    $pl = $this->players[$this->board->side];

    try {
      $output = $pl->requestAction($input);
      $move = $this->getMove($output->text);
      $this->board->makeMove($move);
      $ti = new TurnInfo($move, $output->kibitzes, $pl->lastMoveTime);
      $this->gameInfo->addTurn($ti);
    } catch (AtaxxException $e) {
      $msg = sprintf('Eroare: %s', $e->getMessage());
      $this->gameInfo->disqualify($this->board->side, $msg);
      Log::warn($msg);
      $this->badMove = true;
    }
  }

  private function asInputFile(): string {
    $s = $this->board->asInputFile();
    $s .= sprintf("%d %d\n",
                  $this->players[0]->remainingTime,
                  $this->players[1]->remainingTime);
    return $s;
  }

  private function getMove(string $text): Move {
    $m = new Move($text);
    if (!$this->board->isLegalMove($m)) {
      throw new AtaxxException("Mutarea {$text} este ilegală.");
    }

    return $m;
  }

  function getInfo(): GameInfo {
    return $this->gameInfo;
  }

  private function computeScores(): void {
    $b = $this->board;
    if (!$this->badMove) {
      $pieces = [$b->countSquares(0), $b->countSquares(1)];

      if (!$b->anyMoves()) {
        // Partea care nu este la mutare ocupă toate spațiile rămase.
        $pieces[!$b->side] += $b->countSquares(Board::EMPTY);
      }

      $this->gameInfo->setPieces($pieces);
    }

    $this->print();
    Log::notice('Scor final:');
    for ($i = 0; $i < 2; $i++) {
      Log::notice('%s: %0.1f puncte, %d piese', [
        $this->players[$i]->name,
        $this->gameInfo->getScore($i),
        $this->gameInfo->getPieces($i),
      ]);
    }
  }

  function print(): void {
    $p = $this->players;
    $padLen = Str::maxLength(array_column($p, 'name'));

    $this->board->print();
    for ($i = 0; $i < 2; $i++) {
      Log::debug('%s⬤%s %s    %2d piese %0.3f s', [
        AnsiColors::PIECES[$i],
        AnsiColors::DEFAULT,
        mb_str_pad($p[$i]->name, $padLen),
        $this->board->countSquares($i),
        $p[$i]->remainingTime / 1000,
      ]);
    }
  }
}
