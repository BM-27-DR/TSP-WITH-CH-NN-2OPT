# Function to format each line into {x, y} format
def format_point(line):
    parts = line.split()
    if len(parts) >= 3:
        x = parts[1]
        y = parts[2]
        return f"{{{x}, {y}}},"

# Function to read data from a file and format the points
def process_file(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()
        formatted_points = [format_point(line) for line in lines]
        return formatted_points

# Process each line of data and format the points
formatted_points = process_file('input.txt')  # Replace 'input.txt' with your file name

# Write the formatted points to a file
with open('output.txt', 'w') as file:
    for point in formatted_points:
        if point:
            file.write(point + '\n')