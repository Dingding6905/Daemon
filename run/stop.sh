#!/bin/sh

cd ../run/
if [ -f $1.pid ]
then
    kill `cat ./$1.pid`
    echo "daemon is stop ok"
else
    echo "daemon is not start"
fi
