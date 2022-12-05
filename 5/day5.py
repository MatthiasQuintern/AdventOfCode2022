from re import fullmatch
from sys import argv, exit

re_instruction = r"move (\d+) from (\d+) to (\d+)\n"

MAX_CRATES = 8
STACK_COUNT = 9

def fill_stacks(cratestacks:list[str], lines:list[str]):
    for _ in range(MAX_CRATES):
        line = lines.pop(0).strip('\n')
        if not '[' in line: return
        for i in range(STACK_COUNT):
            # if i * 4 + 1 > len(line):
            #     print(f"Error, i{i} len(line){len(line)}, line {line}")
            if line[i*4] == '[': cratestacks[i] += line[i*4+1]
    for i in range(len(cratestacks)):
        cratestacks[i] = cratestacks[i][::-1]

def crate_mover_9000(cratestacks: list[str], amount:int, fromStack:int, toStack:int):
    # print("9000", cratestacks, amount, fromStack, toStack)
    for _ in range(amount):
        cratestacks[toStack-1] += cratestacks[fromStack-1][-1]
        cratestacks[fromStack-1] = cratestacks[fromStack-1][:-1]

def crate_mover_9001(cratestacks: list[str], amount:int, fromStack:int, toStack:int):
    # print("9001", cratestacks, amount, fromStack, toStack, cratestacks[fromStack-1][(-amount):])
    cratestacks[toStack-1] += cratestacks[fromStack-1][(-amount):]
    cratestacks[fromStack-1] = cratestacks[fromStack-1][:(-amount)]

def main(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()
    cratestacks_1:list[str] = [ "" for _ in range(STACK_COUNT) ]
    fill_stacks(cratestacks_1, lines)
    cratestacks_2:list[str] = list(cratestacks_1)
    for line in lines:
        match = fullmatch(re_instruction, line)
        if not match: continue
        amount, fromStack, toStack = [ int(x) for x in match.groups() ]
        # print(amount, fromStack, toStack)
        crate_mover_9000(cratestacks_1, amount, fromStack, toStack);
        crate_mover_9001(cratestacks_2, amount, fromStack, toStack);
    top_crates_1 = ""
    top_crates_2 = ""
    for i in range(len(cratestacks_1)):
        top_crates_1 += cratestacks_1[i][-1]
        top_crates_2 += cratestacks_2[i][-1]
    print(f"Using the CrateMover 9000, the crates at the top are: {top_crates_1}")
    print(f"Using the CrateMover 9001, the crates at the top are: {top_crates_2}")

if __name__ == '__main__':
    if not len(argv) == 2:
        print(f"Expected exactly one argument (filename), got {len(argv)-1}")
        exit(1)
    main(argv[1])
