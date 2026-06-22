<?php

/**
 * O clasă care salvează informațiile conținute într-un GameInfo.
 **/

class Saver {
  private GameInfo $gameInfo;
  private array $players;
  private int $round;
  private string $saveDir;
  private bool $saveInputs;

  function __construct(GameInfo $gameInfo, array $players, int $round,
                       string $saveDir, bool $saveInputs) {
    $this->gameInfo = $gameInfo;
    $this->players = $players;
    $this->round = $round;
    $this->saveDir = $saveDir;
    $this->saveInputs = $saveInputs;
  }

  function saveAll(): void {
    if ($this->saveDir) {
      $this->saveGame();
      if ($this->saveInputs) {
        $this->saveAllInputs();
      }
    }
  }

  private function saveGame(): void {
    $fileName = $this->getSaveFile();
    $data = $this->gameInfo->asArray();
    $data['players'] = array_column($this->players, 'name');
    $json = json_encode($data);
    file_put_contents($fileName, $json);
  }

  private function saveAllInputs(): void {
    @mkdir($this->getInputDir());

    foreach ($this->gameInfo->getInputs() as $i => $text) {
      $fileName = $this->getInputFile($i);
      file_put_contents($fileName, $text);
    }
  }

  private function getSaveFile(): string {
    $fileName = sprintf(Config::SAVE_GAME_FILE,
                        $this->round + 1,
                        $this->players[0]->name,
                        $this->players[1]->name);
    return "{$this->saveDir}/{$fileName}";
  }

  private function getInputDir(): string {
    $inputDir = sprintf(Config::SAVE_INPUT_DIR,
                        $this->round + 1,
                        $this->players[0]->name,
                        $this->players[1]->name);
    return "{$this->saveDir}/{$inputDir}";
  }

  private function getInputFile(int $i): string {
    $inputDir = $this->getInputDir();
    $inputFile = sprintf(Config::SAVE_INPUT_FILE, $i);
    return "{$inputDir}/{$inputFile}";
  }
}
