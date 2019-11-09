////////////////////////////// INCLUDED LIBRARIES //////////////////////////////

#include <float.h>
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
    coordinate_t best_position;
    double best_fitness;
    double omega;
    double c1, c2;
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

double goldstein_price_function(coordinate_t p) {
    return (1+(p.x+p.y+1)*(p.x+p.y+1)*(19-14*p.x+3*p.x*p.x-14*p.y+6*p.x*p.y+3*p.y*p.y))*(30+(2*p.x-3*p.y)*(2*p.x-3*p.y)*(18-32*p.x+12*p.x*p.x+48*p.y-36*p.x*p.y+27*p.y*p.y));
}

void init_particle(particle_t *p) {
    p->personal_best = DBL_MAX;
}

void randomize_particle(particle_t *particle){
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

void evaluate_particle(particle_t *p) {
    double fitness;

    fitness = goldstein_price_function(p->position);
    if (fitness < p->personal_best) {
        p->personal_best = fitness;
        p->p_best_position = p->position;
    }
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

swarm_t * new_swarm(int n_particles, double omega, double c1, double c2) {
    swarm_t *swarm;
    int i;

    swarm = calloc(1, sizeof(swarm_t));
    swarm->n_particles = n_particles;
    swarm->c1 = c1;
    swarm->c2 = c2;
    swarm->omega = omega;
    swarm->particles = calloc(n_particles, sizeof(particle_t));
    swarm->best_fitness = DBL_MAX;
    for (i = 0; i < n_particles; i++) {
        init_particle(&swarm->particles[i]);
    }
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
    printf("Best position: (%lf %lf)\n", swarm->best_position.x, swarm->best_position.y);
    printf("Best fitness: %lf\n", swarm->best_fitness);
    print_swarm_particles(swarm);
}

void randomize_swarm_particles(swarm_t *swarm) {
    int i;

    for (i = 0; i < swarm->n_particles; i++)
    {
        randomize_particle(&swarm->particles[i]);
    }
    
}

void evaluate_swarm_particles(swarm_t *swarm){
    particle_t *particle;
    int i;

    for (i = 0; i < swarm->n_particles; i++) {
        particle = &swarm->particles[i];
        evaluate_particle(particle);
        if (particle->personal_best < swarm->best_fitness) {
            swarm->best_position = particle->p_best_position;
            swarm->best_fitness = particle->personal_best;
        }
    }
}

void update_swarm_particles(swarm_t *swarm) {
    particle_t *particle;
    int i;

    for (i = 0; i < swarm->n_particles; i++) {
        particle = &swarm->particles[i];
        update_particle(particle, swarm->omega, swarm->c1, swarm->c2, swarm->best_position);
    }
    
}

void pso(swarm_t *swarm, int iterations) {
    int i;

    for (i = 0; i < iterations; i++){
        evaluate_swarm_particles(swarm);
        update_swarm_particles(swarm);
    }
}

///////////////////////////////////// MAIN /////////////////////////////////////

int main(int argc, char const *argv[]) {
    int n_particles;
    double c1, c2;
    double omega;
    int iterations;
    swarm_t *swarm;

    srand(time(NULL));

    if (argc != 6){
        printf("Error: \n");
        exit(1);
    } else {
        n_particles = atoi(argv[1]);
        omega = atof(argv[2]);
        c1 = atof(argv[3]);
        c2 = atof(argv[4]);
        iterations = atoi(argv[5]);
    }

    swarm = new_swarm(n_particles, omega, c1, c2);
    randomize_swarm_particles(swarm);

    pso(swarm, iterations);
    printf("Best position: (%lf %lf)\nBest Fitness: %lf\n", swarm->best_position.x, swarm->best_position.y, swarm->best_fitness);

    free_swarm(swarm);

    return 0;
}
