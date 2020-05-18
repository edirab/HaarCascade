
f = open('bad_navigation_2.dat', 'w')

for i in range(1, 2956):
    s = '../preparing waiter/extracted_images/without/without' + '{:04d}'.format(i) + ".jpg\n"
    f.write(s)

for i in range(1662):
    s = '../preparing waiter/extracted_images/without_advanced/advanced' + '{:04d}'.format(i) + ".jpg\n"
    f.write(s)