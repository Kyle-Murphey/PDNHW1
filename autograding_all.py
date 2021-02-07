import numpy as np
import pandas as pd
import os
import subprocess
import glob

# To use this autograding
# you would need to place your zip file under a "submissions" folder

# Run like this python3 autograding_all.py

target_dir = "./submissions/"

prob_directory = "Problem_1/" 
file_name = "serial_mult_mat_vec.c" 

# Unzipping files

l_zip_files = glob.glob(target_dir+'*.zip')

# Expected name of zip file
# walkerjohn_HW1.zip

for file in l_zip_files:
    print(file)

    # name folders with the name only
    # e.g. walkerjohn
    dir_file_name = file.split('_')[0]
    
    command = "(unzip {} -d {})".format(file ,dir_file_name)
    os.system(command)

# Copy Makefile if not there 

l_directories = glob.glob(target_dir+'*/')

dir_make = "./Makefile"
for dir in l_directories:
    dir_prob1 = dir + "Problem_1/"
    if not os.path.isfile(dir_prob1+"Makefile"):
        command = "(cp {} {})".format(dir_make, dir_prob1)
        os.system(command)

# run make

dir_make = "./Makefile"
for dir in l_directories:
    print(dir)
    dir_prob1 = dir + "Problem_1/"
    command = "(cd {} && make)".format(dir_prob1)
    os.system(command)

# Run the test cases
# serial_mult_mat_vec file_1.csv n_row_1 n_col_1 file_2.csv n_row_2 outputfile.csv

l_test = [["test1_input_mat.csv", 3, 3, "test1_input_vec.csv", 3, "test1_results.csv"],
          ["test2_input_mat.csv", 10, 10, "test2_input_vec.csv", 10, "test2_results.csv"],
          ["test3_input_mat.csv", 100, 100, "test3_input_vec.csv", 100, "test3_results.csv"]]

for dir in l_directories:
    for i in range(len(l_test)):
        # print(dir)
        dir_exe = dir + "Problem_1/serial_mult_mat_vec"
        dir_output = dir + "Problem_1/"
        # print(dir_exe)
        # print("-------------------------------")
        command = "({} {} {} {} {} {} {} )".format(dir_exe, l_test[i][0], l_test[i][1], l_test[i][2],
                                                   l_test[i][3],l_test[i][4],dir_output + l_test[i][5] )
        # print(command)
        os.system(command)

# # print(l_directories)

l_student_names = [dir.split('/')[2] for dir in l_directories]
# # print(l_student_names)

l_col_names = [1,2,3,"total"]

df_grades = pd.DataFrame(np.nan, index=[i for i in l_student_names],
                     columns=[i for i in l_col_names])

# # print(df_grades)

l_files_benchmark = [ "test1_output_vec.csv",
                      "test2_output_vec.csv",
                      "test3_output_vec.csv"]

l_files_results = [ "test1_results.csv",
                    "test2_results.csv",
                    "test3_results.csv"]

for dir in l_directories:
    name_student = dir.split('/')[2]
    count_success = 0
    for i in range(len(l_files_benchmark)):
        # print(dir)
        dir_prob1 = dir + "Problem_1/"
        # print("-------------------------------")
        results_benchmark_test_temp = np.genfromtxt("./" + l_files_benchmark[i], delimiter=',')
        results_test_temp = np.genfromtxt( dir_prob1 + l_files_results[i],  delimiter=',')

        if ( len(results_benchmark_test_temp) != len(results_test_temp) ):
            print("Student:", name_student,"Test", (i+1),"Different size")
            df_grades.loc[name_student, i+1 ] = 0
            continue

        # calculating the sum of relative error
        diff_temp = np.sum( np.absolute ( (results_benchmark_test_temp - results_test_temp)/results_benchmark_test_temp ) )

        # print(diff_temp)
        if diff_temp < 0.1:
            # print("Test", (i+1), "Succeeded")
            df_grades.loc[name_student, i+1 ] = 1
            count_success += 1
        else:
            # print("Test", (i+1), "Failed")
            df_grades.loc[name_student, i+1 ] = 0

    df_grades.loc[name_student, "total" ] = count_success
        
print(df_grades)