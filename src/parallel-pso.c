////////////////////////////// INCLUDED LIBRARIES //////////////////////////////

#include <float.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "particle.h"

////////////////////////////////// FUNCTIONS ///////////////////////////////////

void get_input(int my_rank, double *omega, double *c1, double *c2, double *error){
    if (my_rank == 0){
        if (!scanf("%lf %lf %lf %lf", omega, c1, c2, error)) {
            printf("Error reading input!\n");
            exit(1);
        }
    }
    MPI_Bcast(omega, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(c1, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(c2, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(error, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}

double goldstein_price_function(coordinate_t p) {
    return (1+(p.x+p.y+1)*(p.x+p.y+1)*(19-14*p.x+3*p.x*p.x-14*p.y+6*p.x*p.y+3*p.y*p.y))*(30+(2*p.x-3*p.y)*(2*p.x-3*p.y)*(18-32*p.x+12*p.x*p.x+48*p.y-36*p.x*p.y+27*p.y*p.y));
}

void evaluate_particle(particle_t *p) {
    double fitness;

    fitness = goldstein_price_function(p->position);
    if (fitness < p->personal_best) {
        p->personal_best = fitness;
        p->p_best_position = p->position;
    }
}

///////////////////////////////////// MAIN /////////////////////////////////////

int main(void)
{
    int comm_sz, my_rank;
    double omega;
    double c1,c2;
    double error;
    double best_fitness;
    coordinate_t best_position;
    double aux_fit;
    double aux_pos_x, aux_pos_y;
    int  i, j;

    particle_t particle;

    MPI_Init(NULL, NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    get_input(my_rank, &omega, &c1, &c2, &error);

    srand(time(NULL) + my_rank);

    init_particle(&particle);
    randomize_particle(&particle);

    //o primeiro processo inicia o melhor fit
    best_fitness = DBL_MAX;

    if (my_rank == 0)
    {
        printf("iteration, fitness, x_pos, y_pos\n");
    }

    i = 0;
    while(1){

        evaluate_particle(&particle);

        if (particle.personal_best < best_fitness){
            best_position = particle.p_best_position;
            best_fitness = particle.personal_best;
        }
        
        if (my_rank != 0){
            MPI_Send(&best_fitness, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
            MPI_Send(&best_position.x, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
            MPI_Send(&best_position.y, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        } else {
            for (j = 1; j < comm_sz; j++)
            {
                MPI_Recv(&aux_fit, 1, MPI_DOUBLE, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(&aux_pos_x, 1, MPI_DOUBLE, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(&aux_pos_y, 1, MPI_DOUBLE, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if (aux_fit < best_fitness){
                    best_fitness = aux_fit;
                    best_position.x = aux_pos_x;
                    best_position.y = aux_pos_y;
                }
            }
        }

        MPI_Bcast(&best_fitness, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&best_position.x, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&best_position.y, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        if (my_rank == 0){
            printf("%d, %lf, %lf, %f\n", i, best_fitness, best_position.x, best_position.y);
        }

        if ((best_fitness - 3) < error){
            break;
        }

        update_particle(&particle,omega,c1,c2,best_position);
        i++;
    }
    
    MPI_Finalize();
    return 0;
}