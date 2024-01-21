#!/bin/bash
 
currentDirectory=`pwd`
build=0
time=0
type=""
parts="both"
input=""
compact=0

while getopts ":botci:" option; do
   case $option in
      b)
         build=1;;
      o)
         type="optimized";;
      t)
         time=1;;
      c)
         compact=1;;
      i)
         input=$OPTARG;;
     \?)
         echo "Error: Invalid option"
         exit;;
   esac
done

for arg in "$@"; do
    case $arg in
        [12])
        parts=$arg;;
    esac
done

latestHeaderDate=0

for header in $(find $currentDirectory $currentDirectory/.. $currentDirectory/../.. -maxdepth 1 -name "*.h"); do
    headerDate=$(date -r $header +%s)

    if (($headerDate > $latestHeaderDate)); then
        latestHeaderDate=$headerDate
    fi
done

for (( part=1; part <= 2; part++ )); do 
    if [ $parts == "both" ] || [ $parts == $part ]; then
        partAnswer=`grep -m 1 -o '= [^ *]*' part${part}.c | cut -d " " -f2`
        partSourceDate=$(date -r part${part}.c +%s)
        partObjectDate=0

        if [ -f part${part}${type}.o ]; then
            partObjectDate=$(date -r part${part}${type}.o +%s)
        fi

        if [ $build == 1 ] || [ ! -f part${part}${type}.o ] || (($partSourceDate > $partObjectDate)) || (($latestHeaderDate > $partObjectDate)); then
            if [[ $type == "optimized" ]]; then
                gcc part${part}.c -O3 -o part${part}${type}.o
            else
                gcc part${part}.c -o part${part}${type}.o
            fi

            if [ $? -ne 0 ]; then
                exit
            fi

            partObjectDate=$(date -r part${part}${type}.o +%s)
        fi

        if [ $time == 1 ]; then
            input="$input --time"

            if [ ! -f part${part}.txt ] || (($partObjectDate > $(date -r part${part}.txt +%s))); then
                ./part${part}${type}.o $input > /dev/null 2>&1

                if [ $? -ne 0 ]; then
                    exit
                fi
            fi

            ./part${part}${type}.o $input > part${part}.txt

            output=$(cat part${part}.txt)

            if [ $compact == 1 ]; then
                echo "$output"
            else
                answer=`echo $output | cut -d " " -f 1`
                duration=`echo $output | cut -d " " -f 2`

                echo "Part $part: $answer, time: $duration"
            fi
        else
            ./part${part}${type}.o $input
            printf "\n"
        fi
    fi
done