<?php

class Player {
  private string $binary;
  public string $name;
  public string $score;      // puncte adunate în partidele terminate
  public string $pieces;     // piese adunate în partidele terminate
  public string $numGames;   // numărul de partide terminate
  public int $initialTime;   // timpul inițial în milisecunde
  public int $remainingTime; // timpul rămas în partida curentă
  public int $lastMoveTime;  // timpul petrecut la ultima mutare

  function __construct(string $binary, string $name, int $timeMillis) {
    $this->binary = $binary;
    $this->name = $name;
    $this->score = 0.0;
    $this->pieces = 0;
    $this->numGames = 0;
    $this->initialTime = $timeMillis;
    Log::info('Am adăugat jucătorul %s cu binarul %s.', [ $name, $binary ]);
  }

  function resetClock(): void {
    $this->remainingTime = $this->initialTime;
  }

  function requestAction(string $gameState): Output {
    Log::info('Aștept o acțiune de la %s', [ $this->name ]);
    $inter = new Interactor($this->binary, $gameState);
    $inter->run($this->remainingTime);
    $this->lastMoveTime = min($inter->getTime(), $this->remainingTime);
    $this->remainingTime -= $this->lastMoveTime;
    return $inter->getOutput();
  }

  function addResult(float $score, int $pieces): void {
    $this->score += $score;
    $this->pieces += $pieces;
    $this->numGames++;
  }
}
