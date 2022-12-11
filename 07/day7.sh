#!/bin/bash

# INPUT=$(pwd)/"output.txt"
INPUT=$(pwd)/$1
# ROOT_DIR="/tmp/test/root"
ROOT_DIR="/tmp/day7"

PWD="$ROOT_DIR"

function pwd_() {
    echo $(pwd) | sed "s($ROOT_DIR(("
}

function createFS() {
    mkdir -p "$ROOT_DIR"
    rm -r "$ROOT_DIR"
    mkdir "$ROOT_DIR"
    cd  "$ROOT_DIR"

    while read line; do
        case $line in 
            "$ cd "*)
                dirname=$(echo $line | awk '{print $3}')
                if [[ $dirname == "/" ]]; then
                    cd "$ROOT_DIR"
                    continue
                fi
                # echo ">> cd $dirname"
                mkdir -p $dirname || exit 1
                cd $dirname
                ;;
            "\$ ls")
                # echo ">> ls"
                ;;
            "dir "*)
                # echo ">> dir"
                ;;
            *)  # output of ls
                file=$(echo $line | awk '{print $2}')
                size=$(echo $line | awk '{print $1}')
                fallocate -l $(expr $size) $file
                # touch $file
                ;;
        esac
    done < $INPUT;
}

FS_SIZE=70000000
REQ_SIZE=30000000

function getDirsize() {
    cd "$ROOT_DIR"
    ifs=$IFS; IFS=$'\n'
    du_output=($(du -b --apparent-size .))
    # subtract the size of the directory without its contents from the total size
    # 40 for dir itself
    # 60 for each sub(sub)dir
    # 20 for each file in (sub)dir
    for (( i = 0; i < "${#du_output[@]}"; i++)) do
        line=${du_output[$i]}
        dirname=$(echo $line | awk '{print $2}')
        size=$(echo $line | awk '{print $1}')

        dirsize=$(ls -ld $dirname | awk '{print $5}')  # size of directory without its contents
        subfiles=$(find $dirname -type f | wc -l)
        subdirs=$(find $dirname -type d | wc -l)
        du_output[$i]="$(expr $size + 20 - 60 "*" $subdirs - 20 "*" - $subfiles) $dirname"
    done
    # printf '%s\n' "${du_output[@]}"
    printf '%s\n' "${du_output[@]}" | awk 'BEGIN{sum=0} {if ($1 <= 100000) {sum+=($1)}} END{print "Total size of all dirs<100000: "sum-0}'
    IFS=$ifs
    ROOT_SIZE=$(echo ${du_output[-1]} | awk '{print $1}')
    minSize=$(expr $REQ_SIZE - $FS_SIZE + $ROOT_SIZE)
    # echo "minSize $minSize"

    printf '%s\n' "${du_output[@]}" | awk -v min_size=$minSize 'BEGIN{file=""; size=99999999} {if ($1 >= min_size && $1 < size) {size=$1; file=$2}} END{print "You should delete "file" size("size")"}'

}

createFS
getDirsize
