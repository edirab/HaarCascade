abs_path = 'E:/University/10sem/nirs/haar_3_4_6/preparing data/'

f = open(abs_path+'good.dat')
f_fixed = open('good_fixed.dat', 'w')

data = f.readlines()
print(type(data))

print("Неверно обработанные файлы: ")

for line in data:
    line = line.strip().split('0\\')
    new_line = line[0] + "0/already_processed/" + line[1] + "\n"
    print(new_line, end = '')
    f_fixed.write(new_line)
