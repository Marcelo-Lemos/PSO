////////////////////////////// INCLUDED LIBRARIES //////////////////////////////

#include <float.h>
#include <stdio.h>
#include <stdlib.h>

#include "particle.h"

////////////////////////////////// FUNCTIONS ///////////////////////////////////

double random_range(double min, double max) {
    return (((double)rand() / RAND_MAX) * (max - min)) + min;
}

coordinate_t random_coordinate(double x_min, double x_max, double y_min, double y_max) {
    coordinate_t coordinate;

    coordinate.x = random_range(x_min, x_max);
    coordinate.y = random_range(y_min, y_max);

    return coordinate;
}

void init_particle(particle_t *p) {
    p->personal_best = DBL_MAX;
}

void randomize_particle(particle_t *particle) {
    // TODO: change the way min and max are coded.
    particle->position = random_coordinate(-2, 2, -2, 2);
    particle->velocity = random_coordinate(-0.2, 0.2, -0.2, 0.2);
}

void print_particle(particle_t particle) {
    printf("Position: (%lf %lf)\nVelocity: (%lf %lf)\nBest Position: (%lf %lf)\nPersonal Best: %lf\n",
           particle.position.x,
           particle.position.y,
           particle.velocity.x,
           particle.velocity.y,
           particle.p_best_position.x,
           particle.p_best_position.y,
           particle.personal_best);
}

void update_particle_velocity(particle_t *p, double omega, double c1, double c2, coordinate_t best_position) {
    double random_p, random_g;

    // Velocity x
    random_p = random_range(0, 1);
    random_g = random_range(0, 1);
    p->velocity.x = (omega*p->velocity.x) + (c1*random_p*(p->p_best_position.x - p->position.x)) + (c2*random_g*(best_position.x - p->position.x));

    // Velocity y
    random_p = random_range(0, 1);
    random_g = random_range(0, 1);
    p->velocity.y = (omega*p->velocity.y) + (c1*random_p*(p->p_best_position.y - p->position.y)) + (c2*random_g*(best_position.y - p->position.y));
}

void update_particle_position(particle_t *p) {
    p->position.x += p->velocity.x;
    p->position.y += p->velocity.y;
}

void update_particle(particle_t *p, double omega, double c1, double c2, coordinate_t best_position) {
    update_particle_velocity(p, omega, c1, c2, best_position);
    update_particle_position(p);
}
