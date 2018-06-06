#include <stdio.h>
#include <mpi.h>

#define N 3

int input1[N][N], input2[N][N], output[N][N];
int position, worker, x, y, z;

MPI_Status status;

int main(int argc, const char * argv[]) {
    int rank, amount, rows, position, w, x, y, z;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &amount);

    // rows per worker
    rows = N / (amount - 1);

    // master
    if (rank == 0) {

      // populate inputs
      for (x = 0; x < N; x++) {
        for (y = 0; y < N; y++) {
          input1[x][y] = 1;
          input2[x][y] = 1;
        }
      }

      // send data to workers
      position = 0;
      for (w = 1; w < amount; w++) {
        MPI_Send(&position, 1, MPI_INT, w, 1, MPI_COMM_WORLD);
        MPI_Send(&input1[position][0], rows * N, MPI_INT, w, 1, MPI_COMM_WORLD);
        MPI_Send(&input2, N * N, MPI_INT, w, 1, MPI_COMM_WORLD);
        position = position + rows;
      }

      // receive computed data from workers
      for (w = 1; w < amount; w++) {
        MPI_Recv(&position, 1, MPI_INT, w, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&output[position][0], rows * N, MPI_INT, w, 1, MPI_COMM_WORLD, &status);
      }

      // show computed data
      for (x = 0; x < N; x++) {
        for (y = 0; y < N; y++) {
          printf("%d ", output[x][y]);
        }
        printf("\n");
      }

    }

    // worker
    if (rank > 0) {

      // receive data from master
      MPI_Recv(&position, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
      MPI_Recv(&input1, rows * N, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
      MPI_Recv(&input2, N * N, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

      // compute data
      for (x = 0; x < N; x++) {
        for (y = 0; y < rows; y++) {
          output[y][x] = 0;
          for (z = 0; z < N; z++) {
            output[y][x] = output[y][x] + input1[y][z] * input2[z][x];
          }
        }
      }

      // send computed data to master
      MPI_Send(&position, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
      MPI_Send(&output, rows * N, MPI_INT, 0, 1, MPI_COMM_WORLD);

    }

    MPI_Finalize();

    return 0;
}
