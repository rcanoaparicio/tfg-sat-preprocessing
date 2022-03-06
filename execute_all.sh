#!/bin/bash
make
for FILE in test_files/*.cnf; do
  echo "====================================="
	echo "FILE: $FILE"
	time ./main < $FILE;
  echo "-------------------------------------"
	time ./probing < $FILE | ./main;
  echo "-------------------------------------"
	time ./probing < $FILE | ./bronkerbosch;
  echo "====================================="
done
