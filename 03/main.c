#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define char_bits long unsigned int

int convert_char_to_priority(char c) {
    if (c >= 'a') { 
        return (int) c + 1 - 'a';
    }
    else {
        return (int) c + 27 - 'A';
    }
}

int find_set_bit(const char_bits* bits) {
    for (int bit = 0; bit < 64; bit++) {
        if (*bits & (1ul << bit)) {
            return bit;
        }
    }
    return -1;
}

int main() {
    FILE* p_file;
    p_file = fopen("rucksack_items.txt", "r");
    if (p_file == NULL) {
        printf("Could not open file");
        exit(1);
    }
    int lineCount = 0;
    char* p_linebuffer = NULL;
    size_t linebufferLength = 0;
    ssize_t lineLength = getline(&p_linebuffer, &linebufferLength, p_file);
    int priorities =  0;
    int groupPriorities = 0;
    char_bits bitsLine1, bitsLine2, bitsLine3 = 0; // each bit is one char(item), where a is 1, A is 27

    while (lineLength > 0) {
        char_bits currentBitsHalf1 = 0;
        char_bits currentBitsHalf2 = 0;
        assert(lineLength % 2 == 1);  // even backpacks + null
        lineCount++;
        int backpackLength = (lineLength - 1) / 2;

        char_bits* lineBits = NULL;
        switch(lineCount % 3) {
            case 1: lineBits = &bitsLine1; break;
            case 2: lineBits = &bitsLine2; break;
            case 0: lineBits = &bitsLine3; break;
            default: printf("Error in switch-case"); exit(1); 
        }
        // set all the bits for the correct bitsLine
        for (int i = 0; i < backpackLength; i++) {  // first half
            int prioritiy = convert_char_to_priority(p_linebuffer[i]);
            currentBitsHalf1 |= (1ul << prioritiy);  // set bit of the item
        }
        for (int i = backpackLength; i < 2*backpackLength; i++) {  // second half
            int prioritiy = convert_char_to_priority(p_linebuffer[i]);
            currentBitsHalf2 |= (1ul << prioritiy);
        }

        *lineBits |= currentBitsHalf1 | currentBitsHalf2;
        currentBitsHalf1 &= currentBitsHalf2;
        int priority = find_set_bit(&currentBitsHalf1);
        if (priority < 0) {
            printf("Error in line [%06d]: Could not find duplicate character. Backpack length: [%06d], line content: '%s'\n", lineCount, backpackLength, p_linebuffer);
            exit(1);
        }
        priorities += priority;
        if (lineCount % 3 == 0) {
            bitsLine1 &= bitsLine2 & bitsLine3;
            priority = find_set_bit(&bitsLine1);
            if (priority < 0) {
                printf("Error in group [%06d]: Could not find group badge item type.", lineCount / 3);
                exit(1);
            }
            groupPriorities += priority;
            bitsLine1 = 0; bitsLine2 = 0; bitsLine3 = 0;
        }
        /* printf("line[%06d]: lineSize:[%06zd], priorities:[%06d], groupPriorities:[%06d], content: %s", lineCount, lineLength, priorities, groupPriorities, p_linebuffer); */
        lineLength = getline(&p_linebuffer, &linebufferLength, p_file);
    }
    free(p_linebuffer);
    p_linebuffer = NULL;
    fclose(p_file);

    printf("The sum of priorities of the duplicate items is: %06d\n", priorities);
    printf("The sum of priorities of the group badges is: %06d\n", groupPriorities);
    return 0;
}
