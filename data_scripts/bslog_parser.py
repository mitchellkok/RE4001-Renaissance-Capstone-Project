import os
import os.path

FILENAME = "Exp3_Rooftop.csv"

# Set path of csv file (csv file will be saved in the same location laptop2.py)
path = os.getcwd()
newPath = path.replace(os.sep, '/')
csv_file = newPath + '/' + FILENAME

# Using readlines()
file1 = open(csv_file, 'r')
Lines = file1.readlines()

print(Lines[0:10])
  
# count = 0
# # Strips the newline character
# for line in Lines:
#     count += 1
#     print("Line{}: {}".format(count, line.strip()))