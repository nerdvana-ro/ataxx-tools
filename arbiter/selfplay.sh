#!/usr/bin/bash

ORIG_DIR=`pwd`
STABLE_BIN=../andromeda-stable/build/andromeda
DEV_BIN=../andromeda/build/andromeda
DEV_DIR=../andromeda
SAVE_DIR=~/Desktop/ataxx-selfplay
REPORT_FILE=$SAVE_DIR/report.txt
LOG_FILE=/tmp/ataxx.log

CONFIG_FILE=include/Constants.h
REGEX="s/const int POP_COEF = (.+);/const int POP_COEF = %d;/";
GREP_PATTERN="POP_COEF" # for verification purposes
MIN_VAL=50
MAX_VAL=150
STEP=10

rm -rf $REPORT_FILE

for val in `seq $MIN_VAL $STEP $MAX_VAL`; do
  regex=`printf "$REGEX" $val`
  echo "==== Running substitution: $regex"

  cd $DEV_DIR

  echo "* Modifying the config file"
  sed -i -E "$regex" $CONFIG_FILE
  grep "$GREP_PATTERN" $CONFIG_FILE

  echo "* Building the project"
  cd build
  make

  echo "* Running the tournament"
  cd $ORIG_DIR
  save_dir=$SAVE_DIR/`printf "%04d" $val`
  rm -rf $save_dir
  mkdir -p $save_dir

  php arbiter/tournament.php \
      --binary $STABLE_BIN --name stable \
      --binary $DEV_BIN --name dev_$val \
      --rounds 10 \
      --save $save_dir 2>&1 | tee $LOG_FILE

  tail -n 5 $LOG_FILE >> $REPORT_FILE
done
