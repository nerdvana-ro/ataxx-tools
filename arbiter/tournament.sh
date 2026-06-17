#!/usr/bin/bash

SAVE_DIR=~/Desktop/ataxx-games

rm -rf $SAVE_DIR
mkdir $SAVE_DIR

php tournament.php \
    --binary /calea/spre/binar1 --name nume1 \
    --binary /calea/spre/binar2 --name nume2 \
    --binary /calea/spre/binar3 --name nume3 \
    --binary /calea/spre/binar4 --name nume4 \
    --rounds 6 \
    --save $SAVE_DIR \
    --save-inputs
