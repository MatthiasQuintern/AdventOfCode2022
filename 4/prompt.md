# original prompt
consider a text file like this:
22-65,22-66
91-94,63-91
6-88,5-31
85-86,64-86
40-65,40-65
25-82,24-94

write an gnu-x86 assembly program that:
- reads a text file like this line by line and count how often one of the two ranges contains the other.
- accepts the filename via command line arg
- outputs the result using printf with a suitable string
A range r1 contains the other r2 when the r1.lower <= r2.lower and r1.upper >= r2.upper
the program needs to output how often that is the case in the entire file

# add task 2 to c code:
can you add another check to the c code:
it should now also check if one of the ranges overlaps the other.
A range r1 overlaps the other r2  if:
- r1.lower == r2.lower
- r1.upper == r2.upper
- r1.lower > r2.lower and r1.lower <= r2.upper
- r1.upper < r1.upper r1.upper >= r2.lower
...
sorry i failed to mention that the overlaps and containment should be counted separately
