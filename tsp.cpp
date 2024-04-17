// This program implements the Convex Hull Nearest Neighbor (CH-NN) algorithm for the Traveling Salesman Problem (TSP) and uses the 2-opt heuristic to improve the tour . This file has been modified to find the run time of the program and export data to files for visualization purposes.

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <set>
#include <sstream>
#include <fstream>  // Include the fstream library for file I/O
#include <chrono>   // Include the chrono library for time functions

using namespace std;



//struct to represent 2d point
struct Point {
    double x, y;
};


// Function to calculate the Euclidean distance between two points
double distance(const Point& a, const Point& b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// Function to determine if three points make a counter-clockwise turn
int ccw(const Point& a, const Point& b, const Point& c) {
    double area = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    if (area > 0) return 1;     // Counter-clockwise
    if (area < 0) return -1;    // Clockwise
    return 0;                    // Collinear
}



// Function to calculate the convex hull of a set of points 
vector<Point> convexHull(const vector<Point>& points) {
    int n = points.size();
    if (n < 3) return {}; // Not enough points for a convex hull

    vector<Point> sortedPoints = points;
    sort(sortedPoints.begin(), sortedPoints.end(), [](const Point& a, const Point& b) {
        return a.x == b.x ? a.y < b.y : a.x < b.x;
    });

    vector<Point> upperHull, lowerHull;

    // storing the steps of the upper hull into a file
    ofstream upperHullStepsFile("upper_hull_steps.txt");
    
    for (int i = 0; i < n; ++i) {
        while (upperHull.size() >= 2 && ccw(upperHull[upperHull.size() - 2], upperHull.back(), sortedPoints[i]) <= 0)
            upperHull.pop_back();
        upperHull.push_back(sortedPoints[i]);

        // Output the current state of the upper hull
        for (const Point& p : upperHull) {
            upperHullStepsFile << p.x << " " << p.y << "\n";
        }
        upperHullStepsFile << "\n";
    }

    upperHullStepsFile.close();


    // storing the steps of the lower hull into a file
    ofstream lowerHullStepsFile("lower_hull_steps.txt");
    for (int i = n - 1; i >= 0; --i) {
        while (lowerHull.size() >= 2 && ccw(lowerHull[lowerHull.size() - 2], lowerHull.back(), sortedPoints[i]) <= 0)
            lowerHull.pop_back();
        lowerHull.push_back(sortedPoints[i]);

        // Output the current state of the lower hull
        for (const Point& p : lowerHull) {
            lowerHullStepsFile << p.x << " " << p.y << "\n";
        }
        lowerHullStepsFile << "\n";  // Separate steps with a blank line
    }
    lowerHullStepsFile.close(); // Close the file after writing

    lowerHull.pop_back(); // Remove duplicate points from the lower hull
    upperHull.pop_back(); // Remove duplicate points from the upper hull

    upperHull.insert(upperHull.end(), lowerHull.begin(), lowerHull.end());

    return upperHull;
}




// Function to find the nearest neighbor of a point
int nearestNeighbor(const Point& current, const vector<Point>& points, const set<int>& visited) {
    double minDist = numeric_limits<double>::infinity(); 
    int nearestIdx = -1;

    for (size_t i = 0; i < points.size(); ++i) {
        if (visited.find(i) == visited.end()) { // Check if point is not already visited
            double dist = distance(current, points[i]);
            if (dist < minDist) {
                minDist = dist;
                nearestIdx = static_cast<int>(i);
            }
        }
    }
    return nearestIdx;
}




// Function to solve TSP using CH-NN algorithm
vector<int> chnn(const vector<Point>& points) {
    vector<int> tour;
    vector<Point> hull = convexHull(points); // Compute the convex hull of the points

    set<int> visited;
    tour.reserve(hull.size()); // Reserve space for the tour

    for (size_t i = 0; i < hull.size(); ++i) {
        tour.emplace_back(static_cast<int>(i)); // Cast to int for consistency
        visited.insert(static_cast<int>(i));
    }
    Point current = hull.back();
    while (visited.size() < points.size()) {
        int nearest = nearestNeighbor(current, points, visited);
        tour.emplace_back(nearest); // add the new point to end of the tour
        visited.insert(nearest);
        current = points[nearest];
    }
    return tour;
}




// Function to perform 2-opt heuristic on the tour
vector<int> twoOptSwap(const vector<Point>& points, vector<int>& tour) {
    bool improvement = true;
    while (improvement) {
        improvement = false;
        for (size_t i = 0; i < tour.size() - 1; ++i) {
            for (size_t j = i + 1; j < tour.size(); ++j) {
                double oldDistance = distance(points[tour[i]], points[tour[i + 1 == tour.size() ? 0 : i + 1]]) +
                                      distance(points[tour[j]], points[tour[(j + 1) % tour.size()]]);

                double newDistance = distance(points[tour[i]], points[tour[j]]) +
                                     distance(points[tour[i + 1 == tour.size() ? 0 : i + 1]], points[tour[(j + 1) % tour.size()]]);

                if (newDistance < oldDistance) {
                    reverse(tour.begin() + i + 1, tour.begin() + j + 1);
                    improvement = true; //
                    break;
                }
            }
            if (improvement)
                break;
        }
    }
    return tour;
}





int main() {
    // Read the points from a file
    ifstream file("output.txt");
    string line;
    vector<Point> points;
    
    while (getline(file, line)) {
        istringstream iss(line);
        double x, y;
        char ch1, ch2, ch3;
        if (iss >> ch1 >> x >> ch2 >> y >> ch3) {
            points.push_back({x, y});
        }
    }

    auto start = chrono::high_resolution_clock::now();
    vector<int> tour = chnn(points); 
    tour = twoOptSwap(points, tour); 
    auto end = chrono::high_resolution_clock::now();
    
    // Output the tour to a file
    ofstream outFile("tour.txt");
    for (int cityIndex : tour) {
        Point city = points[cityIndex];
        outFile << city.x << " " << city.y << "\n";
    }
    outFile.close();

    //calculate the total distance of the tour
    double totalDistance = 0.0;
    for (size_t i = 0; i < tour.size(); ++i) {
        Point city = points[tour[i]];
        Point nextCity = points[tour[(i + 1) % tour.size()]]; // Wrap around for last city
        double dist = distance(city, nextCity);
        totalDistance += dist;
    }

    int cnt = 0;
    cout << "CH-NN Tour: ";
    for (size_t i = 0; i < tour.size(); ++i) {
        if(cnt % 10 == 0) cout << endl;
        cout << tour[i]; // Print index of the point in the tour
        if (i != tour.size() - 1) {
            cout << " -> ";
        }
        cnt ++;
    }
    
    cout << endl;
    totalDistance += distance(points[tour.back()], points[tour.front()]);
    cout << "Total Tour Distance: " << totalDistance << endl;

    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start); // Calculate the duration
    cout << "Time taken by program: " << duration.count() << " milliseconds" << endl;

    return 0;
}