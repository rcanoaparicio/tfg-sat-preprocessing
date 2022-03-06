#!/bin/bash
make
rm quick-cliques/data/test_files/*
for FILE in test_files/*.cnf; do
	echo "FILE: $FILE"
	./probing < $FILE | ./graphGenerator > quick-cliques/data/$FILE.gph;
done
./quick-cliques/test.sh
