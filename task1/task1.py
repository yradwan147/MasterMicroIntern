from turtle import Screen
import matplotlib.pyplot as plt
from kivy.lang import Builder
from kivy.app import App
from kivy.garden.matplotlib.backend_kivyagg import FigureCanvasKivyAgg
from kivy.uix.screenmanager import ScreenManager, Screen
import ast
import operator as op
import time

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

# Local memory of figures
figures = []

# First Input Screen
class InputScreen(Screen):
    # Function to evaluate inputs and graph figures. Add or Overlay is dependent on the function bool argument "overlay"
    def graph(self, overlay=False):
        function = self.ids.function.text
        min = self.ids.min.text
        max = self.ids.max.text

        try:
            # Making all x's lower case
            if "**" in function:
                raise Exception("Please use ^ as the power operator")
            function = function.replace('X', 'x').replace('^','**')

            # Evaluating if max and min are numbers. Remove the minus sign in case of negative number to avoid errors.
            if not min.lstrip('-').isnumeric():
                raise Exception("Invalid minimum number")
            if not max.lstrip('-').isnumeric():
                raise Exception("Invalid maximum number")
            
            # Make sure min is less than max
            if int(min) >= int(max):
                raise Exception("Minimum can't be larger than or equal maximum")
            
            # Check that there's only one variable in the function
            letters = set()
            for char in function:
                if char.isalpha():
                    letters.add(char.lower())
            if letters != {"x"}:
                raise Exception("Too many variables")

            # Function Mathematical Evaluation
            try:
                eval_expr(function)
            except ZeroDivisionError as e:
                raise Exception("Function involves dividing by zero")
            except Exception as e:
                raise Exception("Invalid function")
            
            # Create x points for graphing
            x = list(range(int(min),int(max),5))

            # Create y points using list comprehension
            y = [eval(function.replace('x', '('+str(i)+')')) for i in x]

            # Add Figure
            if overlay == False:
                fig1 = plt.figure()
                plt.plot(x,y)
                # Add x and y axes
                plt.axvline(x=0, c="black", label="x=0")
                plt.axhline(y=0, c="black", label="y=0")
                figures.append(fig1)
                self.ids.res.text = "Added to Figures"
            
            # Overlay Figure
            else:
                if len(figures) == 0:
                    raise Exception("You need to add at least one figure to overlay on it")
                plt.figure(figures[-1].number)
                plt.plot(x,y)
                self.ids.res.text = "Overlayed to last figure"
                time.sleep(0.3)
            return True
        except Exception as e:
            # Show error
            self.ids.res.text = "Failure: " + str(e)
            return False

class OutputScreen(Screen):
    def show(self):
        box = self.ids.box

        # Remove figures to avoid duplication then re add stored figures
        box.clear_widgets()
        for figure in figures:  
            box.add_widget(FigureCanvasKivyAgg(figure))

    
    def clear(self):
        box = self.ids.box
        # Clear stored figures and clear shown figures
        figures.clear()
        box.clear_widgets()

    def save(self):
        name = self.ids.namer.text
        # Save figure using name in textinput which has id "namer"
        if name:
            print("saving...")
            plt.savefig(name)

class WindowManager(ScreenManager):
    pass

kv = Builder.load_file("task1.kv")

class MainApp(App):
    def build(self):
        return kv

MainApp().run()