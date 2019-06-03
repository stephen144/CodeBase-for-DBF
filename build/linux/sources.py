group = list()
with open('sources.txt', 'r') as f:
    for line in f:
        group.append(line.strip())
        if len(group) == 4:
            print(' '.join(group), ' \\')
            group = list()
    else:
        print(' '.join(group))