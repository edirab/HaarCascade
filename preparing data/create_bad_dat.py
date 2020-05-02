import os

f = open('bad.dat', 'w')

for i in range(1, 2956):
    s = './extracted_images/without_800/' + '{:04d}'.format(i) + ".jpg\n"
    f.write(s)
    # print(s, end = '')
