#!/bin/sh

#set -e

if [ "./build.sh" == "$0" ]
then
	echo "error: please execute 'source ./build.sh'"
	echo -e ""
	exit
fi

#���������ʱ�ļ� .o �� ��ִ���ļ�Ŀ¼
if [ ! -d ./objs/ ]
then
	mkdir ./objs/
fi

cd ./src/
make
if [ -f ../run/mydaemond ]
then
    \mv ../run/mydaemond ../run/mydaemond.old
fi

\mv ../objs/mydaemond ../run/
\mv ../objs/worker    ../run/
make clean

cd ../bin/
chmod 777 ./mydaemon

cd ../run/
chmod 777 ./stop.sh
chmod 777 ./start.sh
chmod 777 ./cmd.sh
\mv ../bin/mydaemon .
source ./cmd.sh ./mydaemon
\mv ./mydaemon ../bin/

PATH_RUN=`pwd`
STRING_GREP=`grep "$PATH_RUN" /etc/profile`
#û�ҵ�
if [ $? -eq 1 ]
then
    #ɾ��һ��
    sed -i '/export PATH/d' /etc/profile
    echo "export PATH=\$PATH:$PATH_RUN" >> /etc/profile
	source /etc/profile
fi

cd ..

