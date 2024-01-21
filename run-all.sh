#!/bin/bash
 
currentDirectory=`pwd`
years=()
start=0
end=0
resetRange=1

for arg in "$@"; do
    if [[ $arg =~ ^[0-9]{4}$ ]]; then
        years+=($arg)
    elif [[ $arg == [0-9]*..[0-9]* ]]; then
        startLength=`expr "$arg" : '[0-9]*'`
         
        start=${arg:0:$startLength}
         
        let endLength="${#arg}-$startLength-2"
        let endLength="-$endLength"
         
        end=${arg:$endLength}

        resetRange=0
    elif [[ $arg =~ ^[0-9]{1,2}$ ]]; then
        start=$arg
        end=$arg

        resetRange=0
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

widestAnswer=""

for year in ${years[@]}; do
    if [[ ${currentDirectory:${#currentDirectory}-4} != $year ]]; then
        pushd . > /dev/null
        cd $year
    fi

    for answer in $(find ./ -name "part*.c" | xargs grep -m 1 -o '= [^ *]*' | cut -d " " -f2); do 
        if ((${#answer} > ${#widestAnswer})); then
            widestAnswer=$answer
        fi
    done
    
    if ((${#years[@]} > 1)); then
        popd > /dev/null
    fi
done

answerWidth=${#widestAnswer}
allCorrects=()
yearTimes=()
allYearsCorrect="true"
totalTime=0.0

for year in ${years[@]}; do
    if [[ ${currentDirectory:${#currentDirectory}-4} != $year ]]; then
        pushd . > /dev/null
        cd $year
    fi

    if [ $resetRange == 1 ]; then
        start=1
 
        lastDir=`ls -d day* | tail -n 1`
     
        let endLength="${#lastDir}-3"
        let endLength="-$endLength"

        end=${lastDir:$endLength}

        if [[ $end == 0[1-9] ]]; then
            finalEndLength=endLength+1
        
            end=${lastDir:$finalEndLength}
        fi
    fi

    allCorrect="true"
    yearTime=0.0

    if [ $start -le $end ]; then
        if ((${#years[@]} == 1)) || [ $year == ${years[0]} ]; then
            printf "Year/Day/Part %-${answerWidth}s Correct   Time\n" "Answer"
            echo "--------------$(printf "\055%.0s" $(seq 1 $answerWidth))-------------------"
        fi

        for (( number=$start; number <= $end; number++ )); do
            directory="$(printf "day%02d" $number)"
     
            if [ -d $directory ]; then
                pushd . > /dev/null
                cd $directory
                
                for (( part=1; part <= 2; part++ )); do 
                    read -r answer time <<<$(../../run.sh -otc $part)
                     
                    final=`grep -m 1 -o '= [^ *]*' part${part}.c | cut -d " " -f2`

                    if [ "$final" == "$answer" ]; then
                        correct=" true"
                    else
                        if [ "$final" == "" ]; then
                            correct="  N/A"
                        else
                            correct="false"
                            allCorrect="false"
                            allYearsCorrect="false"
                        fi
                    fi

                    yearTime="$( bc <<<"$yearTime + $time" )"

                    printf "$year d%02d p$part:  %-${answerWidth}s   %5s  %9f\n" $number $answer $correct $time
                done

                echo "--------------$(printf "\055%.0s" $(seq 1 $answerWidth))-------------------"
     
                popd > /dev/null
            fi
        done

        totalTime="$( bc <<<"$totalTime + $yearTime" )"
     
        if ((${#years[@]} == 1)) && [ $start -lt $end ]; then
            printf "All           %-${answerWidth}s   %5s  %9f\n" "---" $allCorrect $yearTime
        else
            allCorrects+=($allCorrect)
            yearTimes+=($yearTime)
        fi
    fi
    
    if ((${#years[@]} > 1)); then
        popd > /dev/null
    fi
done

if ((${#years[@]} > 1)); then
    printf "Year      %-${answerWidth}s All Correct   Time\n" ""
    echo "--------------$(printf "\055%.0s" $(seq 1 $answerWidth))-------------------"

    for i in "${!years[@]}"; do
        printf "${years[$i]}          %-${answerWidth}s   %5s  %9f\n" "" ${allCorrects[$i]} ${yearTimes[$i]}
    done

    echo "--------------$(printf "\055%.0s" $(seq 1 $answerWidth))-------------------"
    printf "All Years     %-${answerWidth}s   %5s  %9f\n" "" $allYearsCorrect $totalTime
    echo "--------------$(printf "\055%.0s" $(seq 1 $answerWidth))-------------------"
fi
