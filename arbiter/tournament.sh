#!/usr/bin/bash

SAVE_DIR=~/Desktop/ataxx-games

rm -rf $SAVE_DIR
mkdir $SAVE_DIR

php tournament.php \
    --binary ../agent/doofus/build/doofus --name doofus1 \
    --binary ../agent/doofus/build/doofus --name doofus2 \
    --rounds 1 \
    --save $SAVE_DIR \
    --save-inputs
