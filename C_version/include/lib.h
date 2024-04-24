#ifndef __LIB_H__
#define __LIB_H__


/* Typedef */
typedef struct {
    float x;
    float v;
    float a;
} Particle;


/* Constants */


/* Headers of functions */

float random_float(const float min, const float max);

float U(float x, float* traj, int len_traj);

void velocity_verlet(Particle* ball, float* traj, const int len_traj, const float dx, const float dt, int step, const float x_min, const float x_max);

#endif

