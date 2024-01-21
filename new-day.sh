#!/bin/bash

let day=0

if [ "$1" != "" ]; then
    let day=$1
fi

if [[ $day == 0 ]]; then
    lastDir=`ls -d day* | tail -n 1`

    if [[ $lastDir != "" ]]; then
        let endLength="${#lastDir}-3"
        let endLength="-$endLength"

        let lastDay=${lastDir:$endLength}

        if [[ $lastDay == 0[1-9] ]]; then
            finalEndLength=endLength+1
        
            lastDay=${lastDir:$finalEndLength}
        fi

        day=$((lastDay+1))
    else
        day=1
    fi
fi

if [ $day != 0 ]; then
    dayString=$day

    if [[ $dayString == [0-9] ]]; then
        dayString="0${day}"
    fi

    if [[ $dayString == [0-2][0-9] ]]; then
        mkdir "day${dayString}" 2> /dev/null
        pushd . > /dev/null
        cd "day${dayString}"
    
        for ((part=1; part <= 2; part++)); do
            cat > "part${part}.c" <<EOF
/* Day ${day}, part ${part} = ? */

#include "../../aoc.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        fclose(inputFile);
    }

    print(aoc, INT, &answer);

    return 0;
}
EOF
        done

        popd > /dev/null

        if [ -f $(dirname "$0")/session.txt ]; then
            ../download-input.sh
        else
            touch input.txt
        fi
    fi
fi