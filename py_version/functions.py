""" This module contains all the functions for the metadynamics script """

# Importing modules
import numpy as np
from numba import njit

# Functions

@njit
def U(x, traj) :
    """ Function to evaluate the potential.
        The potential has two terms :
            1) The real function
            2) A sum of gaussians centered in the visited positions (characteristic of metadunamics) """

    # First term
    a = 5
    U =  a * (np.power(x,2) - 2) * np.power(x,2)

    # Second term
    var = 0.1
    A = 0.01

    for i in range(traj.shape[0]) :
        U += A * np.exp( - np.square(traj[i] - x) / (2 * np.square(var)) )

    return U



@njit
def velocity_verlet (x, v, a, traj, dt, dx, x_max, x_min, T, gamma) :
    """ This function upgrades positions and velocities of the particles using the velocity
        verlet algorithm. This function makes use of calculate_forces """

    eta = np.random.normal(loc=0.0, scale=1.0, size=1)

    v_dt_2 = v + 0.5 * a * dt                                               # Update velocities pt.1
    x = x + v_dt_2 * dt                                                     # Update position
    F_cons = - (U(x+dx, traj) - U(x-dx, traj)) / (2 * dx)                   # Conservative forces
    F_frict = - gamma * v_dt_2                                              # Friction
    F_brown = np.sqrt(2 * gamma * T) * 2 * eta                              # Brownian motion
    a = F_cons + F_frict + F_brown                                          # Update forces
    v = v_dt_2 + 0.5 * a * dt                                               # Update velocities pt.2

    # PBC
    if x > x_max :
        x -= 2 * x_max
    if x < x_min :
        x += 2 * x_max

    return x, v, a


    