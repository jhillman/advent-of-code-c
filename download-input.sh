#!/bin/bash

if [ ! -f $(dirname "$0")/session.txt ]; then
	echo "session.txt does not exist"
	exit
fi

session=$(cat $(dirname "$0")/session.txt) 
currentDirectory=`pwd`
years=()

for arg in "$@"; do
    if [[ $arg =~ ^[0-9]{4}$ ]]; then
        years+=($arg)
    fi
done

if ((${#years[@]} == 0)); then
    if [[ $currentDirectory =~ [0-9]{4}$ ]]; then
        years=(${currentDirectory:${#currentDirectory}-4})
    else
        for year in $(ls -A1 -d -- [0-9][0-9][0-9][0-9] | sort); do
            years+=($year)
        done
    fi
fi

for year in ${years[@]}; do
    if [[ ${currentDirectory:${#currentDirectory}-4} != $year ]]; then
        pushd . > /dev/null
        cd $year
    fi

    for directory in $(find ./ -type d -name "day*" | sort); do
        pushd . > /dev/null
        cd $directory

		if [ $(find . -maxdepth 1 -name "input*.txt" | wc -l) == 0 ]; then
            day=`grep -m 1 -o '[0-9]\+' part1.c | head -1`
			echo "downloading input for $year day $day..."
    
            curl -s https://adventofcode.com/$year/day/$day/input --cookie "session=$session" >> input.txt
            truncate -s -1 input.txt
    	fi

        popd > /dev/null
    done

    if ((${#years[@]} > 1)); then
        popd > /dev/null
    fi
done