#!/bin/sh

#����ִ���������Զ���ȫ����
source ./options
#�� ��ȫ���� �� �ļ�����ʾ����
#complete -F _options -A file $1
complete -F _options $1
