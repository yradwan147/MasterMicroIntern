import unittest

import ast
import operator as op
# supported operators
operators = {ast.Add: op.add, ast.Sub: op.sub, ast.Mult: op.mul,
             ast.Div: op.truediv, ast.Pow: op.pow, ast.USub: op.neg}

# Function to evaluate mathematical experession. Python built-in eval is very flawed security-wise.
def eval_(node):
    if isinstance(node, ast.Num): # <number>
        return node.n
    elif isinstance(node, ast.BinOp): # <left> <operator> <right>
        return operators[type(node.op)](eval_(node.left), eval_(node.right))
    elif isinstance(node, ast.UnaryOp): # <operator> <operand> e.g., -1
        return operators[type(node.op)](eval_(node.operand))
    else:
        return False

def eval_expr(expr):
    return eval_(ast.parse(expr, mode='eval').body)


def input_error(function, min, max):

    if "**" in function:
        return ("Please use ^ as the power operator")

    function = function.replace('X', 'x').replace('^','**')

    # Evaluating if max and min are numbers. Remove the minus sign in case of negative number to avoid errors.
    if not min.lstrip('-').isnumeric():
        return ("Invalid minimum number")
    if not max.lstrip('-').isnumeric():
        return ("Invalid maximum number")
    
    # Make sure min is less than max
    if int(min) >= int(max):
        return ("Minimum can't be larger than or equal maximum")
    
    # Check that there's only one variable in the function
    letters = set()
    for char in function:
        if char.isalpha():
            letters.add(char.lower())
    if letters != {"x"}:
        return ("Too many variables")

    # Function Mathematical Evaluation
    try:
        eval_expr(function)
    except ZeroDivisionError as e:
        return ("Function involves dividing by zero")
    except Exception as e:
        return ("Invalid function")
    
    return "Valid"
    
def gen_points(function, min, max):
    function = function.replace('X', 'x').replace('^','**')
    # Create x points for graphing
    x = list(range(int(min),int(max)+5,5))

    # Create y points using list comprehension
    y = [eval(function.replace('x', '('+str(i)+')')) for i in x]
    return (y[0],y[-1])

class TestFunctions(unittest.TestCase):

    def test_invalid_min(self):
        self.assertEqual(input_error("x^2", "wrong", "100"), "Invalid minimum number", "Should be invalid minimum")

    def test_invalid_max(self):
        self.assertEqual(input_error("x^2", "-100", "wrong"), "Invalid maximum number", "Should be invalid maximum")
    
    def test_max_bigger_than_min(self):
        self.assertEqual(input_error("x^2", "-100", "-200"), "Minimum can't be larger than or equal maximum", "Should be invalid since maximum is less than minimum")
    
    def test_multi_variable(self):
        self.assertEqual(input_error("x^2+y^2", "-100", "100"), "Too many variables", "Should be invalid due to multiple variables")
    
    def test_function_zero(self):
        self.assertEqual(input_error("1/x", "-100", "100"), "Function involves dividing by zero", "Should be invalid due to division by zero")
    
    def test_function_invalid(self):
        self.assertEqual(input_error("1/*x", "-100", "100"), "Invalid function", "Should be invalid due to 2 operators in succession")
    
    def test_function_invalid2(self):
        self.assertEqual(input_error("2x", "-100", "100"), "Invalid function", "Should be invalid due to lack of operator")
    
    def test_function_invalid3(self):
        self.assertEqual(input_error("2**x", "-100", "100"), "Please use ^ as the power operator", "Should be invalid due to wrong power operator (should be ^)")
    
    def test_function_valid(self):
        self.assertEqual(input_error("2*x + 1/(x+1)", "-100", "100"), "Valid", "Should be valid")

    def test_points(self):
        self.assertEqual(gen_points("x^2", "-100", "100"), (100*100, 100*100), "Should be 10000 for min and max (even function)")
    
    def test_points2(self):
        self.assertEqual(gen_points("x^3", "-100", "100"), (-100*-100*-100, 100*100*100), "Should be 1000000 and -1000000 for min and max (odd function)")

if __name__ == '__main__':
    unittest.main()