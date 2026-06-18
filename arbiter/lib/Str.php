<?php

class Str {

  static function startsWith(string $string, string $prefix): bool {
    $start = substr($string, 0, strlen($prefix));
    return $start == $prefix;
  }

  static function maxLength(array $str): int {
    $len = 0;
    foreach ($str as $s) {
      $len = max($len, mb_strlen($s));
    }
    return $len;
  }

  static function coordsToSquare(string $s): int {
    $col = ord($s[0]) - ord('a');
    $row = ord($s[1]) - ord('1');
    return $row * Config::BOARD_SIZE + $col;
  }

  static function charPlusInt(string $s, int $i): string {
    return chr(ord($s) + $i);
  }
}
