# Convex hull algorithms - Graham Scan + Gift Wrapping (Jarvis March)

This repository contains code for two implementations of convex hull algorithms, namely Graham Scan and Gift Wrapping. It contains two files, one written in C++ which contains the code for both algorithms and another file written in Python which is used for plotting the results.

The purpose of this repo is to showcase the algorithms.

No points need to be supplied to the algorithms since internally it uses a random point generator.

When the C++ file is compiled (compiler not provided) and executed it has two modes of operation for each of the algorithms, which the user is prompted to choose.

### One time run mode
This mode will run the selected algorithm only once, in which case the user is asked to input the desired number of points. This mode will create a file with the name `<algorithm>_hull.txt`. 
#### .txt file data structure
The data in this .txt file is structured in a way so it can be used by the Python script in order to plot all the points and also plot the resultant hull points. The first line of the .txt file represents how many points are in total (N points). The second lines and onwards until reaching the Nth line represents all the points in the set. After those N number of lines, the lines that follow contain the points which are part of the convex hull.

### Evaluation mode
This mode records the time it takes for one of selected algorithms to run. After the user selected an algorithm, be it Graham Scan or Gift Wrapping, the user needs to input:
- The starting number of points
- The step size
- Number of iterations to run

After the timing is done, it records the time taken in seconds into a `<algorithm>_time.txt` file.

#### .txt file data structure
Each row in the file represents how long did the algorithm take to complete for that specific iteration.

#### plotting.py
This python script when ran will ask the user if they want to plot the resultant hull from the Graham Scan algorithm, from the Gift Wrapping algorithm, or if they want to plot the running times of both of these algorithms. In order to do any plotting, the user must run the algorithms in either the one time run mode, or evaluation mode and then respectfully choose the corresponding option when asked for by plotting.py


I hope this repo is of use and any contributions to it are welcomed.
