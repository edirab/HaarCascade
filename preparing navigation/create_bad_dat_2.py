import os

f = open('bad_navigation.dat', 'w')

for i in range(1, 2956):
    s = '../preparing data/extracted_images/without_800/without' + '{:04d}'.format(i) + ".jpg\n"
    f.write(s)
    # print(s, end = '')
