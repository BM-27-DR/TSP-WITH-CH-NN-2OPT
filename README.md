This program implements the Convex Hull Nearest Neighbor (CH-NN) algorithm for solving the Traveling Salesman Problem (TSP) using C++. It also utilizes the 2-opt heuristic to further optimize the tour obtained from the CH-NN algorithm.

# Overview : 
The Convex Hull Nearest Neighbor (CH-NN) algorithm is a heuristic approach to the TSP that involves finding the convex hull of the given points and then iteratively connecting the points in a way that minimizes travel distance. The 2-opt heuristic is then applied to refine the tour.

# Features : 
Convex Hull Computation: The program calculates the convex hull of the given set of 2D points using andrew monotone algorithm. CH-NN Algorithm: Implements the CH-NN heuristic to generate an optimal tour. 2-opt Optimization: Applies the 2-opt heuristic to improve the optimal tour by iteratively swapping edges to reduce the total distance.

# Usage : 
Data Preparation: Navigate to the Dataset folder. Choose any data file containing 2D points. Copy the data from the selected file.

Preprocess the input using the data_pre_process.py After executing the data_pre_process.py the preprocessed data will be present in the input.txt

Executing the TSP Solver: Compile and run the tsp.cpp file which contains the main algorithm. This file displays the optimal tour, optimal tour distance and the time taken in the terminal and feeds this data to the respective files for visualisation

# Visualisation: 
The convexhull and the optimal TSP tour can be visualised using the convexhull_visualisation.py and
tsp_visualisation.py respectively.

# Dependencies
Python, matplotlib,
C++ 14
