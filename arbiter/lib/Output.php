<?php

/**
 * Ieșirea unui agent, compusă din
 * - un vector de întregi (mutarea)
 * - un vector de string-uri (mesajele chibițate)
 */

class Output {
  public string $contents;
  public array $tokens;
  public array $kibitzes;

  function __construct($contents, $kibitzes) {
    $this->contents = $contents;
    $this->tokens = preg_split('/\s+/', $contents, -1, PREG_SPLIT_NO_EMPTY);
    $this->kibitzes = $kibitzes;
  }
}
