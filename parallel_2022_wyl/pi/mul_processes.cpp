#include <iostream>
#include <mpi.h>

#define N 1000000000

int main(){
    MPI_Init(NULL, NULL);
    int proc_size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    double pi=0;
    for (int i=rank;i<N;i+=proc_size){
        if (i%2==0){
            pi+=1/double(2*i+1);
        }else{
            pi+=-1/double(2*i+1);
        }
    }
    pi=4*pi;
    double pi_sum;
    MPI_Reduce(&pi,&pi_sum,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
    if (rank==0){
        std::cout<<pi_sum<<std::endl;
    }
    MPI_Finalize();
}