#!/bin/bash

IWCONFIG=`whereis iwconfig | awk '{printf $2}'`
IFCONFIG=`whereis ifconfig | awk '{printf $2}'`
AIRMON_NG=`whereis airmon-ng | awk '{printf $2}'`

check_tool_path()
{
	local var=$1
	local tool=$2
	ls $var > /dev/null 2>&1
	local inside=$?


	if [ "$var" == "" ] || [ $inside != "0" ]; then
		echo "$tool": command not found
		exit 2
	fi
}

check_tool_path $IWCONFIG iwconfig
check_tool_path $IFCONFIG ifconfig
check_tool_path $AIRMON_NG airmon-ng


case "$1" in
	start)
		if [ "$2" != "" ]; then
			ifnames=$2
		else
			ifnames=`$IWCONFIG 2>/dev/null | grep "IEEE 802.11" | awk '{print $1}'`
		fi
		# set monitor mode
		for ifname in $ifnames; do
			$IFCONFIG $ifname down
			echo $ifname | grep -q mon > /dev/null 2>&1
			ismon=$?
			echo $ifname $ismon
			if [ $ismon -ne 0 ]; then
				$AIRMON_NG start $ifname > /dev/null 2>&1 || exit 2
			fi
		done

		# interface up
		ifnames=`$IWCONFIG 2>/dev/null | grep "IEEE 802.11" | awk '{print $1}'`
		for ifname in $ifnames; do
			echo $ifname | grep -q mon > /dev/null 2>&1
			ismon=$?
			if [ $ismon -eq 0 ]; then
				$IFCONFIG $ifname up > /dev/null 2>&1 || exit 2
			fi
		done
	;;
	stop)
		if [ "$2" != "" ]; then
			ifnames=$2
		else
			ifnames=`$IWCONFIG 2>/dev/null | grep "IEEE 802.11" | awk '{print $1}'`
		fi
		# unset monitor mode
		for ifname in $ifnames; do
			echo $ifname | grep -q mon > /dev/null 2>&1
			ismon=$?
			if [ $ismon -eq 0 ]; then
				$AIRMON_NG stop $ifname > /dev/null 2>&1 || exit 2
			fi
		done
	;;
	restart)
		stop
		start
	;;
	names)
		ifnames=`$IWCONFIG 2>/dev/null | grep "IEEE 802.11" | awk '{print $1}'`
		for ifname in $ifnames; do
			echo $ifname
		done
	;;
	*)
		echo $"Usage: $0 {start|stop|restart|names}"
		exit 2
esac
exit 0
