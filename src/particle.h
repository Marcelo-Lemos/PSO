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

////////////////////////////////// FUNCTIONS ///////////////////////////////////

double random_range(double min, double max);

coordinate_t random_coordinate(double x_min, double x_max, double y_min, double y_max);

void init_particle(particle_t *p);

void randomize_particle(particle_t *particle);

void print_particle(particle_t particle);

void update_particle_velocity(particle_t *p, double omega, double c1, double c2, coordinate_t best_position);

void update_particle_position(particle_t *p);

void update_particle(particle_t *p, double omega, double c1, double c2, coordinate_t best_position);
