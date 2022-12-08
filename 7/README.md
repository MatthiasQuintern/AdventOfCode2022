# [Day 7](https://adventofcode.com/2022/day/7) 
:gift::gift::gift::gift::gift::gift::gift:

Today's language: **Bash** + **awk**

## Day 7 is stupid
The way the challenge is laid out is absolutely ridiculus; both tasks could be done in one-liners on a Linux machine *(which the device from the challenge probably is)*
in any POSIX shell, 
*if* the user wasn't as stupid...

That's why I wrote a bash script that actually creates the whole filesystem from the input-file.
It does so (by default) in `/tmp`. The sizes for the files are only allocated so that `du` shows the filesize, no write operations are taking place.

### I am stupid
The problem is, that (even empty) directories have a size, which also increases with every file and subdirectory in it.
That's why I needed to calculate the sizes of the directories *without* the actual file sizes and subtract that from the output of `du`.
This new output can then be piped into an `awk` one-liner. In reality, this problem wouldn't exist since you would want the total size of a directory anyway.

If the size of the directories themselves would not matter, the commands would be:
```shell
# Task 1
du -b / | awk 'BEGIN{sum=0} {if ($1 <= 100000) {sum+=($1)}} END{print "Total size of all dirs<100000: "sum-0}'
# Task 2: min_size = (Update size) - (file system size) - (used size)
du -b / | awk 'BEGIN{file=""; size=99999999} {if ($1 >= min_size && $1 < size) {size=$1; file=$2}} END{print "You should delete "file" size("size")"}'
```

## Run today's code
```shell
./day7.sh output.txt
```
