#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>
#include "lib.h"

/* Functions body */

float random_float(const float min, const float max){

    /* Function to pick a random float number between min and max */

    float random_number = rand() * (max - min) / RAND_MAX;
    return random_number + min;

}


float U(float x, float* traj, int len_traj) {

    float U = 0;

    /* Parameters */
    float a = 5;
    float var = 0.1;
    float A = 0.01;

    U +=  a* (pow(x, 2) - 2) * pow(x, 2);

    for (int i=0; i<len_traj - 1; i++) {
        U += A * exp( - pow(x - traj[i], 2) / (2 * pow(var, 2)) );
    }

    return U;
}


void velocity_verlet(Particle* ball, float* traj, const int len_traj, const float dx, const float dt, int step, const float x_min, const float x_max) {

    /* Velocity Verlet for NVT with Langevin thermostat */
    float v_dt_2 = 0;
    float gamma = 0.75;
    float T = 2;

    v_dt_2 = ball->v + 0.5 * ball->a * dt;
    ball->x = ball->x + v_dt_2 * dt;
    ball->a = - ( U(ball->x + dx, traj, len_traj) - U(ball->x - dx, traj, len_traj) ) / (2 * dx);       /* Conservative forces */
    ball->a -= gamma * v_dt_2;                                                                          /* Friction */
    ball->a += sqrt(2 * gamma * T) * random_float(-1,1);                                                /* Random force (brownian motion) */
    ball->v = v_dt_2 + 0.5 * ball->a * dt;
    if (ball->x > x_max) {
        ball->x -= 2*x_max;
    }
    if (ball->x < x_min) {
        ball->x += 2*x_max;
    }
}
