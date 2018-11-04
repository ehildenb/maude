#!/bin/bash

DIR=$(dirname "$(readlink -f "$0")")

pushd "$PWD"

cd "$DIR"

run-test() {
  maude -no-banner -no-wrap $1.maude &> $1.out
  if [ $? -ne 0 ]; then
     echo "maude died!"
     return 1
  fi
  local OUT=$(diff "$1.expected" "$1.out" 2>&1)
  if [ ! -z "$OUT" ]; then
    echo "$1 failed"
    return 1
  fi 
}

FAIL=0

for maudefile in $(ls *.maude); do
  name="${maudefile%.*}"
  echo "$name"
  run-test "$name"
  FAIL+=$?
done

popd

exit $FAIL
