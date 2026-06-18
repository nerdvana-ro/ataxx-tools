<?php

/**
 * O clasă care stochează informații despre o mutare pentru a o salva într-un
 * GameInfo.
 **/

class TurnInfo {
  public Move $move;       // mutarea făcută
  public array $kibitzes;  // un vector de stringuri chibițate de agent
  public float $time;      // timpul folosit în milisecunde

  function __construct(Move $move, array $kibitzes, float $time) {
    $this->move = $move;
    $this->kibitzes = $kibitzes;
    $this->time = $time;
  }

  function asArray(): array {
    return [
      'move' => $this->move->toString(),
      'kibitzes' => $this->kibitzes,
      'time' => $this->time,
    ];
  }
}
