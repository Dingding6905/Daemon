#!/bin/sh

DAEMON_NAME=$1

#∆Ù∂Ø√¸¡Ó
if [ "$2" == "start" ] || [ "$2" == "" ]
then

	$DAEMON_NAME  :master process `pwd`
	if [ $? -eq 0 ]
	then

		if [ -f ./$DAEMON_NAME.pid ]
		then
			echo -e "\n $DAEMON_NAME start failed"
			exit
		else
			echo -e "\n $DAEMON_NAME start success"
		fi

	fi
fi

exit

