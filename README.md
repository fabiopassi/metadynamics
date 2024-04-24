# metadynamics

This repo contains the necessary files to perform the simulation of a 1D metadynamics on a single particle.

## Physics background

This simulation involves a particle moving in a 1D double-well potential.

The chosen ensemble is the NVT and the temperature is fixed with a Langevin thermostat. Of course, the thermal energy is not high enough to let the particle overcome the barrier in a reasonable amount of time.

To make the particle overcome the barrier, metadynamics is employed, an enhanced sampling technique: the main idea is that we want to make the already visited positions less probabile, in order to encourage the particle to visit new states; in this case, this is done by adding small gaussians centered in the already-visited positions to the potential.

As the particle explores one well, we add more and more gaussians to the potential, causing the energy of the particle to increase. Once the potential is high enough, the particle is able to overcome the barrier and visit the other well.

To easily visualize what is happening, the particle may be seen like a ship in a dry well: to overcome the barrier and escape from it, we need to add more and more water under the ship, so that it can sail and overcome the barrier.

Another interesting aspect is that the amount of water added to escape the well is proportional to its depth, hence an estimate of the initial potential can be obtained by the final total addedd potential.

## Technical details: C version

The routines in the `C_version` directory are written in C. All the necessary libraries should already be installed on your system.

To compile and run the simulation (only gcc is required), open the directory containing this file in the terminal and type the following commands:

```bash
cd C_version
make
cd build
./exe
```

At the end of the execution, in the build directory you will find three additional files, namely `ref_pot.txt`, `metadyn_pot.txt` and `U_hist.txt`:

* The first two contain the data to visualize the metadynamics simulations; to plot them, the `plot.sh` script can be used (`gnuplot` is required). To do so, simply open the build directory in the terminal and type:

```bash
chmod +x plot.sh
./plot.sh
```

* The last file contains the total added potential, which is an estimate of the "negative" of the initial potential profile. You can plot it with your favourite tool; if you have gnuplot installed, simply type in the terminal:

```bash
gnuplot
plot 'U_hist.txt' w l lt 1 lw 2
```

Type `quit` in the terminal to exit `gnuplot`.

> **Warning**: these commands have been tested on Ubuntu 22.04, but they should work on any (not too bizarre) linux system. Nothing is guaranteed on Windows.

## Technical details: python version

The scripts in the `py_version` directory are written in python. The only additional packages required to run the simulation are `numpy`, `numba` and `matplotlib`; if you have conda installed, the command:

```bash
conda create -n metadyn numba matplotlib
```

should create an environment with all the necessary packages.

After this, you can start the simulation with the commands:

```bash
conda activate metadyn
cd py_version
python main.py
```

## Contact

If you have any doubt or if you spot mistakes/bugs, feel free to contact [fabio.passi24@gmail.com](fabio.passi24@gmail.com)
