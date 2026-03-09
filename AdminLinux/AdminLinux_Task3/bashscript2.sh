#!/bin/bash

BASHRC_EXIST="~/.bashrc"

if [ -f "$BASH_EXIST"]; then
	echo "we found .bashrc and will creat the variables"
	export HELLO=HOSTMAN
	LOCAL=(whoami)
fi

gnome-terminal 

