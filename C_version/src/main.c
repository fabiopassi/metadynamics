#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include "lib.h"

/* 1D metadynamics */

int main (int argc, char **argv) {
	
	/* Variables */
	float x_min = -1.5;													/* Lower extremum */
	float x_max = -x_min;												/* Upper extremum */
	float dt = 0.001;													/* Time step */
	float dx = 0.0001;													/* Step for the calculation of the derivative */
	int num_steps = 300000;												/* Number of MD steps */
	Particle* ball = (Particle*) malloc(sizeof(Particle));				/* Particle moving in the potential */
	int len_traj = 0;													/* Length of the trajectory */
	float* traj = (float*) malloc(num_steps * sizeof(float));			/* Trajectory */
	int num_points = 1000;												/* Number of points for the plot */
	float* x_axis = (float*) malloc(num_points * sizeof(float));
	float* U_plot = (float*) malloc(num_points * sizeof(float));

	/* Reference potential plot */
	FILE* ref_pot = fopen("ref_pot.txt", "w");
	if (ref_pot == NULL) {
		fprintf(stderr, "Error: file not correctly opened.\n");
		exit(EXIT_FAILURE);
	}
	
	for (int n = 0; n < num_points; n++) {
		x_axis[n] = x_min + n * (x_max - x_min) / (num_points-1);
		U_plot[n] = U(x_axis[n], traj, 0);
		fprintf(ref_pot, "%f\t%f\n", x_axis[n], U_plot[n]);
	}

	fclose(ref_pot);
	free(U_plot);
	free(x_axis);


	/* Putting the ball in one minimum and setting velocity to 0.5 */
	srand(time(NULL));

	ball->x = 1;
	traj[len_traj] = ball->x;
	++len_traj;
	ball->v = 0.5;
	ball->a = - ( U(ball->x + dx, traj, len_traj) - U(ball->x - dx, traj, len_traj) ) / (2 * dx);

	/* Time evolution */
	FILE* metadynamics_pot = fopen("metadyn_pot.txt", "w");
	FILE* U_histogram = fopen("U_hist.txt", "w");
	if (metadynamics_pot == NULL) {printf("Error opening the file.\n\n"); exit(EXIT_FAILURE);}
	if (U_histogram == NULL) {printf("Error opening the file.\n\n"); exit(EXIT_FAILURE);}

	int place_gaussian_time = 50;										/* Time every which I place a gaussian */
	int sampling_time = 200;											/* Time every which I print the ball position on the output file */
	int num_points_hist = (int) sqrt( num_steps / sampling_time );		/* Number of points for the histogram plot of the visited coordinates */
	float* x_axis_hist = malloc(num_points_hist * sizeof(*x_axis));
	int* hist_cols = malloc(num_points_hist * sizeof(*hist_cols));
	float bin_width = (x_max - x_min) / (num_points_hist - 1);

	for(int i = 0; i < num_points_hist; i++) {
		hist_cols[i] = 0;
		x_axis_hist[i] = x_min + i * bin_width;
	}

	for (int n = 1; n < num_steps-1; n++) {

		velocity_verlet(ball, traj, len_traj, dx, dt, n, x_min, x_max);

		if (n % place_gaussian_time == 0) {
			/* Add gaussian */
			traj[len_traj] = ball->x;
			++len_traj;
			/* Add +1 in the correct histogram bin */
			hist_cols[(int)((ball->x - x_min) / bin_width)] += 1;
		}

		if (n % sampling_time == 0) {
			/* Print position on the output file */
			fprintf(metadynamics_pot, "%f\t%f\n", ball->x, U(ball->x, traj, len_traj));
		}
		
	}

	for (int i = 0; i < num_points_hist; i++) {
		fprintf(U_histogram, "%f\t%d\n",x_axis_hist[i] , hist_cols[i]);
	}
	
	fclose(metadynamics_pot);
	fclose(U_histogram);

	/* Free variables */
	free(traj);
	free(hist_cols);
	free(ball);
	free(x_axis_hist);
	
	return 0;
}

