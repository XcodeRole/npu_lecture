#include <iostream>
#include <omp.h>
#include <chrono>
#define N 1000000000
using namespace std;
using namespace chrono;
int main(){
    double pi=0;
    auto t0=steady_clock::now();
#pragma omp parallel 
{
    #pragma omp for reduction(+:pi) schedule(static)
    for (int i=0;i<N;i+=8){
        double tmp1=1/double(2*(i)+1);
        double tmp2=1/double(2*(i+2)+1);
        double tmp3=1/double(2*(i+4)+1);
        double tmp4=1/double(2*(i+6)+1);
        double tmp5=tmp1+tmp2+tmp3+tmp4;
        pi+=tmp5;
    }
    #pragma omp for reduction(+:pi) schedule(static)
    for (int i=1;i<N;i+=8){
        double tmp1=1/double(2*(i)+1);
        double tmp2=1/double(2*(i+2)+1);
        double tmp3=1/double(2*(i+4)+1);
        double tmp4=1/double(2*(i+6)+1);
        double tmp5=tmp1+tmp2+tmp3+tmp4;
        pi+=-tmp5;
    }
}
    pi=4*pi;
    auto t1=steady_clock::now();
    cout<<"computation time:"<<duration_cast<milliseconds>(t1-t0).count()<<"ms"<<endl;
    cout<<"pi:"<<pi<<std::endl;
}