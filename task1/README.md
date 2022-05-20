## Python Plotter

This application can be used to graph any user-defined function as long as it obeys a small set of rules:
- Any multiplication must be explicit. 2*x instead of 2x
- The written function must not divide by zero. 1/(x+1) instead of 1/x
- Allowed operators are + - / * ^

After defining the function and the desired minimum and maximum points, the user can then either add the figure or overlay the figure.
Adding the figure shows the figure side-by-side with any previously added figures. Overlaying the figure adds the figure on top of the last-added figure, however this requires at least one created figure to work.
The user can add a function and then click "Back" to go back to add or overlay another.
The user can use the "Clear" button to clear all figures.
The user can use the "Save Figure" button to save the last created figure locally. Do have in mind that the filename that is entered should include the file extension i.e. "test.jpg" not just "test"

Any errors will be shown in the first window console in case of failure.