import shutil

abs_path = 'E:/University/10sem/nirs/haar_3_4_6/preparing navigation/'
which_file = 2

if which_file == 1:
    f = open(abs_path+'good_1.dat')
    f_fixed = open(abs_path+'good_1_fixed.dat', 'a')
else:
    f = open(abs_path + 'good_2.dat')
    f_fixed = open(abs_path + 'good_2_fixed.dat', 'a')

data = f.readlines()
print(type(data))

for line in data:

    first_part = line.strip().split()[0]
    #print(first_part)
    file_name = first_part.split("\\")[-1]
    print(file_name)

    if which_file == 1:
        try:
            shutil.move(abs_path+"extracted_images/with_original/"+file_name, abs_path+"extracted_images/with_original/already_processed/"+file_name)
            line = line.strip().split('\\')
            new_line = line[0] + "/already_processed/" + line[1] + "\n"
            print(new_line, end='')
            f_fixed.write(new_line)
        except FileNotFoundError:
            print(file_name, " does not exist")
            input()
    else:
        try:
            shutil.move(abs_path+"extracted_images/with_original/"+file_name, abs_path+"extracted_images/with_original/already_processed_2/"+file_name)
            line = line.strip().split('\\')
            new_line = line[0] + "/already_processed_2/" + line[1] + "\n"
            print(new_line, end='')
            f_fixed.write(new_line)

        except FileNotFoundError:
            print(file_name, " does not exist")
            input()
