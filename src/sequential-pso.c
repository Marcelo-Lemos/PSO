////////////////////////////// INCLUDED LIBRARIES //////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

////////////////////////////// TYPES AND STRUCTS ///////////////////////////////

typedef struct {
    double x;
    double y;
} coordinate_t;

typedef struct {
    coordinate_t position;
    coordinate_t velocity;

    coordinate_t p_best_position;
    double personal_best;
} particle_t;

typedef struct {
    int n_particles;
    particle_t *particles;
    double c1, c2;
    double omega;
    double phi_p, phi_g;
} swarm_t;

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

void randomize_particle(particle_t *particle){
    // TODO: change the way min and max are coded.
    particle->position = random_coordinate(-2, 2, -2, 2);
    particle->velocity = random_coordinate(-2, 2, -2, 2);
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

swarm_t * new_swarm(int n_particles, double omega, double c1, double c2, double phi_p, double phi_g) {
    swarm_t *swarm;

    swarm = malloc(sizeof(swarm_t));
    swarm->n_particles = n_particles;
    swarm->c1 = c1;
    swarm->c2 = c2;
    swarm->omega = omega;
    swarm->phi_p = phi_p;
    swarm->phi_g = phi_g;
    swarm->particles = calloc(n_particles, sizeof(particle_t));
    return swarm;
}

void free_swarm(swarm_t *swarm) {
    free(swarm->particles);
    free(swarm);
}

void print_swarm_particles(swarm_t *swarm){
    int i;

    for (i = 0; i < swarm->n_particles; i++) {
        printf("\n//--------- Particle %d ---------\\\\\n", i);
        print_particle(swarm->particles[i]);
        printf("\\\\------------------------------//\n\n");
    }
}

void print_swarm(swarm_t *swarm) {
    printf("Swarm size: %d\n", swarm->n_particles);
    printf("Acceleration coefficients: (%lf %lf)\n", swarm->c1, swarm->c2);
    printf("Inertia: %lf\n", swarm->omega);
    printf("Phi values: (%lf %lf)\n", swarm->phi_p, swarm->phi_g);
    print_swarm_particles(swarm);
}

void randomize_swarm_particles(swarm_t *swarm) {
    int i;

    for (i = 0; i < swarm->n_particles; i++)
    {
        randomize_particle(&swarm->particles[i]);
    }
    
}

double goldstein_price_function(coordinate_t p) {
    return (1+(p.x+p.y+1)*(p.x+p.y+1)*(19-14*p.x+3*p.x*p.x-14*p.y+6*p.x*p.y+3*p.y*p.y))*(30+(2*p.x-3*p.y)*(2*p.x-3*p.y)*(18-32*p.x+12*p.x*p.x+48*p.y-36*p.x*p.y+27*p.y*p.y));
}

///////////////////////////////////// MAIN /////////////////////////////////////

int main(int argc, char const *argv[]) {
    int n_particles;
    double c1, c2;
    double omega;
    double phi_personal, phi_global;
    swarm_t *swarm;

    srand(time(NULL));

    if (argc != 7){
        printf("Error: \n");
        exit(1);
    } else {
        n_particles = atoi(argv[1]);
        omega = atof(argv[2]);
        c1 = atof(argv[3]);
        c2 = atof(argv[4]);
        phi_personal = atof(argv[5]);
        phi_global = atof(argv[6]);
    }

    swarm = new_swarm(n_particles, omega, c1, c2, phi_personal, phi_global);
    randomize_swarm_particles(swarm);
    print_swarm(swarm);
    free_swarm(swarm);

    return 0;
}
