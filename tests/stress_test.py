from string import ascii_letters, digits
from random import choice

a = ascii_letters[:10] + 3*digits
b = ""
c = 0

# Print iterations progress
def printProgressBar (iteration, total, prefix = '', suffix = '', decimals = 1, length = 100, fill = '█', printEnd = "\r"):
    percent = ("{0:." + str(decimals) + "f}").format(100 * (iteration / float(total)))
    filledLength = int(length * iteration // total)
    bar = fill * filledLength + '-' * (length - filledLength)
    print(f'\r{prefix} |{bar}| {percent}% {suffix}', end = printEnd)
    if iteration == total: 
        print()

for i in range(104857600):
    if i % 1048576 == 0:
        printProgressBar(i, 104857600, prefix = 'Generating 100 MB file:', suffix = 'Complete', decimals = 0, length = 30)
    b += choice(a)

with open("test100mb.txt", "w") as f:
    f.write(b)
