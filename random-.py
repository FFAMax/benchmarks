import random 

with open('output-py.txt', 'w') as f:
    for i in range(1000000):
        number = 900000000 + random.randint(0, 99999999)
        f.write(f'{number}\n')

