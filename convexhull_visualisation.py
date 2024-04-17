import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Read the hull steps from the files
def read_steps(filename):
    steps = []
    current_step = []
    with open(filename, 'r') as f:
        for line in f:
            if line.strip():
                x, y = map(float, line.strip().split())
                current_step.append((x, y))
            else:
                steps.append(current_step)
                current_step = []
    return steps
# reads the steps from the file
upper_hull_steps = read_steps('upper_hull_steps.txt')
lower_hull_steps = read_steps('lower_hull_steps.txt')
# Create the plot
fig, ax = plt.subplots()

upper_line, = ax.plot([], [], 'b-')
upper_scatter = ax.scatter([], [], color='red')
lower_line, = ax.plot([], [], 'g-')
lower_scatter = ax.scatter([], [], color='blue')
all_points_scatter = ax.scatter([], [], color='black')
# Initialize the plot
def init():
    all_steps = upper_hull_steps + lower_hull_steps
    x_min = min(p[0] for step in all_steps for p in step) - 200  # Decrease minimum x
    x_max = max(p[0] for step in all_steps for p in step) + 200  # Increase maximum x
    y_min = min(p[1] for step in all_steps for p in step) - 200  # Decrease minimum y
    y_max = max(p[1] for step in all_steps for p in step) + 200  # Increase maximum y
    ax.set_xlim(x_min, x_max)
    ax.set_ylim(y_min, y_max)
    all_points = [p for step in all_steps for p in step]
    all_points_scatter.set_offsets(all_points)
    return upper_line, upper_scatter, lower_line, lower_scatter, all_points_scatter,
# Update the plot
def update(frame):
    if frame < len(lower_hull_steps):
        x, y = zip(*lower_hull_steps[frame])
        lower_line.set_data(x, y)
        lower_scatter.set_offsets(lower_hull_steps[frame])
    elif frame < len(lower_hull_steps) + len(upper_hull_steps):
        x, y = zip(*upper_hull_steps[frame - len(lower_hull_steps)])
        upper_line.set_data(x, y)
        upper_scatter.set_offsets(upper_hull_steps[frame - len(lower_hull_steps)])
    else:
        all_points = [p for step in upper_hull_steps + lower_hull_steps for p in step]
        all_points_scatter.set_offsets(all_points)
    return upper_line, upper_scatter, lower_line, lower_scatter, all_points_scatter,

# Add 50 extra frames at the end to create a pause
ani = FuncAnimation(fig, update, frames=range(len(lower_hull_steps) + len(upper_hull_steps) + 50), init_func=init, blit=True)

plt.show()