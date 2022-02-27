#!/bin/bash
make
for FILE in robertoBenchs/*; do
  echo "====================================="
	echo "FILE: $FILE"
	time ./main < $FILE;
  echo "-------------------------------------"
	time ./probing < $FILE | ./main;
  echo "====================================="
done
