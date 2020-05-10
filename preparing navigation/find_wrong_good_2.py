abs_path = 'E:/University/10sem/nirs/haar_3_4_6/preparing data/'

f = open(abs_path+'good.dat')

data = f.readlines()
print(type(data))

print("Неверно обработанные файлы: ")

for line in data:
    line = line.strip()

    if ".jpg 0" in line:
        print(line)