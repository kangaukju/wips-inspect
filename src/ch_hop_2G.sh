#!/bin/bash

prev=1
cur=1
ifca="wlp5s0mon"

while [ true ]
do
	if [ $cur -eq 14 ]; then
		cur=1
	fi
	iwconfig $ifca channel $cur
	echo "ch:$prev --> $cur"

	prev=$cur
	cur=`expr $cur + 1`
	sleep 1
done
