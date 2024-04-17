import matplotlib.pyplot as plt

# Read the tour from the file
tour = []
with open('tour.txt', 'r') as f:
    for line in f:
        x, y = map(float, line.strip().split())
        tour.append((x, y))

# Add the first city to the end of the tour to close the loop
tour.append(tour[0])

# Separate the x and y coordinates
x, y = zip(*tour)

# Plot the tour
plt.figure(figsize=(10, 10))
plt.plot(x, y, 'b-')
plt.scatter(x, y, color='red')
plt.show()