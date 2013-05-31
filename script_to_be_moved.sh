#!/bin/sh

#DIR_PATH=/home/user/contiki-2.6/examples/Pebble_Rigidity_Check_Contiki/
DIR_PATH=../../../examples/Pebble_Rigidity_Check_Contiki/
rm $DIR_PATH\sender.c
#bash $DIR_PATH./script.sh
gcc $DIR_PATH\main.c -o $DIR_PATH\random_generator
$DIR_PATH\./random_generator
cp temp.txt $DIR_PATH
cat $DIR_PATH\pattern1 >> $DIR_PATH\sender.c
cat $DIR_PATH\temp.txt >> $DIR_PATH\sender.c
cat $DIR_PATH\pattern2 >> $DIR_PATH\sender.c
make -C $DIR_PATH all
