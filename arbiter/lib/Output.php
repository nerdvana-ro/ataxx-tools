<?php

/**
 * Ieșirea unui agent, compusă din
 * - un vector de întregi (mutarea)
 * - un vector de string-uri (mesajele chibițate)
 */

class Output {
  public string $text;
  public array $kibitzes;

  function __construct($text, $kibitzes) {
    $this->text = $text;
    $this->kibitzes = $kibitzes;
  }
}
