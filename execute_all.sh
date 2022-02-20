#!/bin/bash
make
for FILE in robertoBenchs/*; do
	echo "FILE: $FILE"
	time ./probing < $FILE | ./main;
done
