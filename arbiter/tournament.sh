#!/usr/bin/bash

# Rulează acest fișier ca arbiter/tournament.sh din directorul ataxx-tools/
# (rădăcina repoului).

# ATENȚIE: vom ȘTERGE și vom recrea acest director.
SAVE_DIR=~/Desktop/ataxx-games

# O listă de identificatori dintre cei definiți mai jos în AGENT
declare -a PLAYERS=(
  "andromeda"
  "SLC"
)

# Comanda de invocat. Parametrizează aici numărul de runde, timpul etc.
cmd="php arbiter/tournament.php
        --rounds 1
        --save $SAVE_DIR
        --save-inputs
        --time 60"

# Lista completă de agenți
declare -A AGENT

# Clienți de test (random, greedy, erori etc.)
AGENT["doofus"]="agent/doofus/build/doofus"
AGENT["echo"]="agent/echo/echo"
AGENT["error"]="agent/error/error"
AGENT["greedy"]="agent/greedy/greedy"
AGENT["hang"]="agent/hang/hang"
AGENT["silent"]="agent/silent/silent"

# Jucător uman
AGENT["Karen"]="human"

# Implementări propriu-zise
AGENT["andromeda"]="../andromeda/build/andromeda"
AGENT["andromeda-stable"]="../andromeda-stable/build/andromeda"
AGENT["Ascend"]="../agents/Ascend-flaviu/main"
AGENT["AtaXXL"]="../agents/AtaXXL-andrei-horia/ataxxl"
AGENT["Dictator"]="../agents/dictator-rareș"
AGENT["Fluxx"]="../agents/fluxx-traian/fluxx"
AGENT["Lorenzo"]="../agents/lorenzo-matei-radu/lorenzo"
AGENT["Spamtenna"]="../agents/spamtenna-alex-cezar"
AGENT["SLC"]="../agents/sardine-la-conservă-andrei-maria/main"

# Completează comanda cu binarele și cu numele jucătorilor.
for p in "${PLAYERS[@]}"; do
  if ! [[ -v AGENT[$p] ]]; then
    echo "EROARE: jucătorul $p nu are un binar definit"
    exit;
  fi
  cmd="${cmd} --binary ${AGENT[$p]} --name ${p}"
done

# Apelurile efective.
echo Șterg și recreez $SAVE_DIR
rm -rf $SAVE_DIR
mkdir $SAVE_DIR

echo Rulez comanda: $cmd
$cmd
