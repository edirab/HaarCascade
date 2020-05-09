abs_path = 'E:/University/10sem/nirs/haar_3_4_6/navigation/'

f = open(abs_path+'good_1.dat')
f_fixed = open(abs_path+'good_1_fixed.dat', 'a')

data = f.readlines()
print(type(data))

for line in data:
    line = line.strip().split('\\')
    new_line = line[0] + "/already_processed/" + line[1] + "\n"
    print(new_line, end = '')
    f_fixed.write(new_line)
