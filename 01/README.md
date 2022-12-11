# [Day 1](https://adventofcode.com/2022/day/1) 
:gift:

Today's language: **awk** + **GNU-coretils**

- Task 1: We use awk to find the maximum while we iterate through the file
- Task 2: We use awk again for summing the calories, then sort them and take the top 3 which we again sum up with awk
```shell
# Find the Elf carrying the most Calories. How many total Calories is that Elf carrying?
awk -f day1-1.awk input.txt
# Find the top three Elves carrying the most Calories. How many Calories are those Elves carrying in total?
./day1.2.sh
```
