from sys import argv
from re import finditer

ROUND_COUNT = 10000

class Monkey:
    monkeys = []
    def __init__(self, start_items:list[int], divisible_by:int, monkey_on_true: int, monkey_on_false: int, operation: str ):
        self.items = start_items
        self.div_by = divisible_by
        self.monkey_on_true = monkey_on_true
        self.monkey_on_false = monkey_on_false
        self.operation = lambda x : x
        self.inspections = 0
        if operation.startswith('*'):
            try:
                i = int(operation[2:])
                self.operation = lambda x : x * i
            except:
                self.operation = lambda x : x * x
        else:
            try:
                i = int(operation[2:])
                self.operation = lambda x : x + i
            except:
                self.operation = lambda x : x + x
        Monkey.monkeys.append(self)
    def do_business(self):
        for _ in range(len(self.items)):
            self.items[0] = self.operation(self.items[0])# // 3
            if self.items[0] % self.div_by == 0:
                Monkey.monkeys[self.monkey_on_true].recv_item(self.items.pop(0))
            else:
                Monkey.monkeys[self.monkey_on_false].recv_item(self.items.pop(0))
            self.inspections += 1

    def recv_item(self, item: int):
        self.items.append(item)
    def get_inspections(self):
        return self.inspections

re_monkey = r"Monkey (\d+):\n *Starting items: (.*)\n *Operation: new = old (.*)\n *Test: divisible by (\d+)\n *If true: throw to monkey (\d+)\n *If false: throw to monkey (\d+)"

def do_monkey_business(filename):
    with open(filename, "r") as file:
        content = file.read()
    for match in finditer(re_monkey, content):
        g = match.groups()
        print(g)
        Monkey([int(x) for x in g[1].replace(" ", "").split(",")], int(g[3]), int(g[4]), int(g[5]), g[2])

    for r in range(ROUND_COUNT):
        print(f"Round {r}")
        for i in range(len(Monkey.monkeys)):
            Monkey.monkeys[i].do_business()

    ranking = [m.get_inspections() for m in Monkey.monkeys]
    ranking.sort()
    result = ranking[-2] * ranking[-1]
    print(ranking)
    print(f"Monkey business level at {result}")

if __name__ == '__main__':
    if not len(argv) == 2:
        print(f"Expected exactly one argument (filename), got {len(argv)-1}")
        exit(1)
    do_monkey_business(argv[1])
