import matplotlib.pyplot as plt

# The format of the <algorithm>_hull .txt files is at follows:
# first line represents how many points are in total (n)
# after the first line, the next (n) lines all represent the Points with their x,y coordinates
# after the (n) lines each lines represents a point on the convex hull

jarvis_march_running_time = r'jarvis_time.txt'
graham_scan_running_time = r'graham_time.txt'


def plot_algorithm_points(algorithm, file):
    with open(file, encoding='utf-8', mode='r')as f:
        lines = f.readlines()
    nm_points = int(lines[0])
    x_values = []
    y_values = []
    for i in range(1, nm_points+1):
        xy_coords = lines[i].strip('\n').split(',')
        x_values.append(float(xy_coords[0]))
        y_values.append(float(xy_coords[1]))
    plt.plot(x_values, y_values, 'bo')
    x_values.clear()
    y_values.clear()

    for i in range(nm_points+1, len(lines)):
        if lines[i] == '':
            continue
        xy_coords = lines[i].strip('\n').split(',')
        x_values.append(float(xy_coords[0]))
        y_values.append(float(xy_coords[1]))
    plt.plot(x_values, y_values, 'r,-')
    plt.savefig(algorithm+'_plot.png')


def plot_running_times():
    with open(jarvis_march_running_time, 'r', encoding='utf-8')as f:
        lines = f.readlines()
    jarvis_nm_points = []
    jarvis_running_time_s = []
    for line in lines:
        data = line.split(',')
        jarvis_nm_points.append(int(data[0]))
        jarvis_running_time_s.append(int(data[1]))

    with open(graham_scan_running_time, 'r', encoding='utf-8')as f:
        lines = f.readlines()
    graham_nm_points = []
    graham_running_time_s = []
    for line in lines:
        data = line.split(',')
        graham_nm_points.append(int(data[0]))
        graham_running_time_s.append(int(data[1]))
    plt.title("Graham Scan - Jarvis March comparison")
    plt.xlabel("Number of points")
    plt.ylabel("Running time (s)")
    plt.plot(jarvis_nm_points, jarvis_running_time_s, label='Jarvis March')
    plt.plot(graham_nm_points, graham_running_time_s, label='Graham Scan')
    plt.legend(loc="upper left")
    plt.savefig('time_plot.png')

print("Hi, enter 1 to plot Graham Scan convex hull, 2 for Jarvis March and 3 to plot the time comparasion between the two.")
while True:
    answer = input()
    if answer.isdigit():
        choice = int(answer)
        if choice == 1:
            plot_algorithm_points('graham', 'graham_hull.txt')
            break
        if choice == 2:
            plot_algorithm_points('jarvis', 'jarvis_hull.txt')
            break
        if choice == 3:
            plot_running_times()
            break
    else:
        print("Invalid input. Try again")
