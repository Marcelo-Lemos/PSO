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

void get_input(int my_rank, double *omega, double *c1, double *c2){
    if (my_rank == 0){
        if (!scanf("%lf %lf %lf", omega, c1, c2)) {
            printf("Error reading input!\n");
            exit(1);
        }
    }
    MPI_Bcast(omega, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(c1, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(c2, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}

///////////////////////////////////// MAIN /////////////////////////////////////

int main(void)
{
    int comm_sz, my_rank;
    double omega;
    double c1,c2;

    particle_t particle;

    MPI_Init(NULL, NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    get_input(my_rank, &omega, &c1, &c2);

    srand(time(NULL) + my_rank);

    init_particle(&particle);
    randomize_particle(&particle);

    MPI_Finalize();
    return 0;
}