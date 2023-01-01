#include <iostream>
#include <omp.h>
#define N 1000000000
int main(){
    double pi=0;
    #pragma omp parallel for reduction(+:pi) num_threads(4)
    for (int i=0;i<N;i++){
        if (i%2==0){
            pi+=1/double(2*i+1);
        }else{
            pi+=-1/double(2*i+1);
        }
    }
    pi=4*pi;
    std::cout<<pi<<std::endl;
}