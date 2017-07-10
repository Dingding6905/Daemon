#!/bin/sh

#用于执行命令行自动补全命令
source ./options
#将 补全命令 和 文件都显示出来
#complete -F _options -A file $1
complete -F _options $1
