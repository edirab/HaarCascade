import shutil

abs_path = 'E:/University/10sem/nirs/haar_3_4_6/preparing navigation/'

f = open(abs_path+'good_1.dat')
f_fixed = open(abs_path+'good_1_fixed.dat', 'a')

data = f.readlines()
print(type(data))

for line in data:
    line = line.strip()

    first_part = line.split()[0]
    #print(first_part)
    file_name = first_part.split("\\")[-1]
    print(file_name)

    try:
        shutil.move(abs_path+"extracted_images/with_original/"+file_name, abs_path+"extracted_images/with_original/already_processed/"+file_name)
    except FileNotFoundError:
        print(file_name, " does not exist")

for line in data:
    line = line.strip().split('\\')
    new_line = line[0] + "/already_processed/" + line[1] + "\n"
    print(new_line, end = '')
    f_fixed.write(new_line)