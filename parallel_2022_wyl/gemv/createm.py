import random

print(8000)

for i in range(0,8000):
    for j in range (0,8000):
        print(random.uniform(0,100),end=" ")
    print()
for j in range (0,8000):
    print(random.uniform(0,10),end=" ")