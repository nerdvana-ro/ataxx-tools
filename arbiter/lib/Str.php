<?php

class Str {

  static function startsWith(string $string, string $prefix): bool {
    $start = substr($string, 0, strlen($prefix));
    return $start == $prefix;
  }

  static function maxLength(array $str) {
    $len = 0;
    foreach ($str as $s) {
      $len = max($len, mb_strlen($s));
    }
    return $len;
  }
}
