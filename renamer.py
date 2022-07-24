import os

file_list = os.listdir()

for f in file_list:
    if len(f) > 16:
        if f[12:16] == '_to_':
            os.rename(f, f[17:20] + ".txt")