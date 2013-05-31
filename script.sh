#!/bin/sh
rm sender.c
gcc main.c -o random_generator
./random_generator
cat pattern1 >> sender.c
cat temp.txt >> sender.c
cat pattern2 >> sender.c
make -f /home/user/contiki-2.6/examples/Pebble_Rigidity_Check_Contiki/Makefile all
