# [Day 3](https://adventofcode.com/2022/day/3) 
:gift::gift::gift:

Today's language: **C**

All the characters (the Elf's items) occuring in (half) a line (backpack) are stored as bits in an unsigned long, duplicate characters can then be determined by bitwise and-ing these numbers.
```shell
# Find the item type that appears in both compartments of each rucksack. What is the sum of the priorities of those item types?
gcc main.c -o day3
./day3
```
