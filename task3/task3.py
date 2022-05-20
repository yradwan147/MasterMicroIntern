def flatten_matrix():

    rows = input("Enter number of rows: ")
    matrix = []
    print('Enter your row element lists one by one separated by commas i.e "[2,3,5],[3,4],[4,7,8],[5,8,9,10]", can be any number of channels and any number of elements per row')
    # Convert string input into list
    for i in range(eval(rows)):
        input_list = eval('['+input('')+']')
        matrix.append(input_list)

    print(matrix)

    out = []

    # Recursive function to pull out all matrix elements and put them into out
    def ravel(in_list):
        for elem in in_list:
            # if the element is a list, recall the function on it
            if isinstance(elem, list):
                ravel(elem)
            # else, add it to out
            else:
                out.append(elem)
    
    # Run the function on inputted matrix
    ravel(matrix)

    return out

print(flatten_matrix())


# Wasn't used because it doesn't support different number of elements or channels in each row. 
# Plus, the above implementation is already a similar implementation to the code under the wrapper function ravel() in numpy
# res = np.array(matrix, dtype=object).ravel()