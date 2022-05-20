import numpy as np

rows = input("Enter number of rows: ")
matrix = []
print('Enter your row element lists one by one separated by commas i.e "[2,3,5],[3,4],[4,7,8],[5,8,9,10]", can be any number of channels and any number of elements per row')
for i in range(eval(rows)):
    input_list = eval('['+input('')+']')
    matrix.append(input_list)

print(matrix)

out = []

def ravel(in_list):
    for elem in in_list:
        if isinstance(elem, list):
            ravel(elem)
        else:
            out.append(elem)

ravel(matrix)

print(out)

# Wasn't used because it doesn't support different number of elements or channels in each row. 
# Plus, the above implementation is already a similar implementation to the code under the wrapper function ravel()
# res = np.array(matrix, dtype=object).ravel()
# print(res[0], res[5], res[8])