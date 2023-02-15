import os
import os.path

FILENAME = "Exp3_BSLog.log"

# Set path of csv file (csv file will be saved in the same location laptop2.py)
path = os.getcwd()
newPath = path.replace(os.sep, '/')
csv_file = newPath + '/' + FILENAME

# Using readlines()
file1 = open(csv_file, 'r')
lines = file1.readlines()

for i in range(len(lines)):
    if lines[i] == "":
        pass
  
# count = 0
# # Strips the newline character
# for line in Lines:
#     count += 1
#     print("Line{}: {}".format(count, line.strip()))