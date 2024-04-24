# * SCRIPT FOR METADYNAMICS  *

# Importing modules
from functions import *
import numpy as np
import matplotlib.pyplot as plt


# Variables
x_min = -1.6                # Lower boundary
x_max = 1.6                 # Upper boundary
dt = 0.001                  # Verlet step
t_gauss = 50                # Number of steps every which I sample
dx = 0.0001                 # Step to evaluate the derivative of the potential energy
num_steps = 300000          # Number of evolution steps

# Thermostat variables
T = 2
gamma = 0.75

# Setting initial position at 1 and velocity random between -1 and 1
x = 1
v = (np.random.rand(1) - 0.5) * 2
traj = np.array([x])


gamma = 0.75
T = 2
F_cons = - (U(x+dx, traj) - U(x-dx, traj)) / (2 * dx)                   # Conservative forces
F_frict = - gamma * v                                                   # Friction
F_brown = np.sqrt(2 * gamma * T) * 2 * (np.random.rand() - 0.5)         # Brownian motion
a = F_cons + F_frict + F_brown                                          # Update forces
    


# Time evolution via velocity-verlet algorithm
for t in range(num_steps) :
     
    # velocity_verlet
    x, v, a = velocity_verlet(x, v, a, traj, dt, dx, x_max, x_min, T, gamma)

    # Sampling position
    if t % t_gauss == 0 :
        traj = np.append(traj, x)


# Animated plot
dx_plot = 0.01
x_axis = np.arange(x_min, x_max, dx_plot)
U_plot = U(x_axis, traj[:1])

# to run GUI event loop
plt.ion()
 
# here we are creating sub plots
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(20, 15))
ax1.plot(x_axis, U_plot, color = "blue")
point = ax1.scatter(traj[0], U(traj[0], traj[:1]), color = "red")

# setting title
plt.title("Metadynamics", fontsize=20)
 
# setting x-axis label and y-axis label
fig.supxlabel("x", fontsize=15)
fig.supylabel("U", fontsize=15)

# Bins for histogram
bins_hist = np.linspace(x_min, x_max, 11)
*trash, my_hist = ax2.hist(traj[:1], bins=bins_hist, color = "red", ec="orange", lw=2 )
ax2.set_xlim((x_min, x_max))
ax2.set_ylim((0, num_steps/200))

# Loop
step_plot = 10
for i in range(0, len(traj), step_plot):

    # updating metadynamics point
    point.remove()
    point = ax1.scatter(traj[i], U(traj[i], traj[:i]), color = "red")

    # updating histogram
    if i % int(step_plot * 5) == 0 :
        my_hist.remove()
        *trash, my_hist = ax2.hist(traj[:i], bins=bins_hist, color = "red", ec="orange", lw=2 )

    fig.canvas.flush_events()

plt.pause(5)
