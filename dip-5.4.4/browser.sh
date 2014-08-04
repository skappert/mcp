#!/bin/bash

if [ -z $DIPBASE ]; then
	echo DIPBASE not set, please source the DIP environment - execute "source setenv.sh"
    exit 1
fi

echo Starting DIP Browser v5.4.4
java -Djava.library.path=$DIPBASE/lib -jar tools/dip-browser-5.4.4.jar &

