#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent a range in the file
struct range {
    int lower;
    int upper;
};

// Function to parse a line from the file and return the two ranges in the line
void parse_line(const char *line, struct range *range1, struct range *range2) {
    // Parse the two ranges in the line
    sscanf(line, "%d-%d,%d-%d", &range1->lower, &range1->upper, &range2->lower, &range2->upper);
}

int main(int argc, char *argv[]) {
    // Check if a filename was provided
    if (argc < 2) {
        fprintf(stderr, "Error: No filename provided\n");
        return 1;
    }

    // Open the file for reading
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Unable to open file for reading\n");
        return 1;
    }

    // Read the file line by line
    char line[256];
    int contain_count = 0;
    int overlap_count = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        // Parse the two ranges in the line
        struct range range1, range2;
        parse_line(line, &range1, &range2);

        // Check if one range contains the other
        if (range1.lower <= range2.lower && range1.upper >= range2.upper) {
            contain_count++;
        }
        else if (range2.lower <= range1.lower && range2.upper >= range1.upper) {
            contain_count++;
        }

        // Check if one range overlaps with the other
        else if (range1.lower == range2.lower ||
                 range1.upper == range2.upper ||
                 (range1.lower > range2.lower && range1.lower <= range2.upper) ||
                 (range1.upper < range2.upper && range1.upper >= range2.lower)) {       // had to fix range1.upper < range2.upper comparison
            overlap_count++;
        }
    }

    // Close the file
    fclose(fp);

    // Print the results
    printf("The file contains %d lines where one of the two ranges contains each other\n", contain_count);
    printf("The file contains %d lines where the two ranges overlap each other\n", overlap_count);
    return 0;
}
