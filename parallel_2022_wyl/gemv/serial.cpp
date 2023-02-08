#include <iostream> 
#include <fstream>
#include <time.h>
#include <cstdlib>
// #include <string>
// #include <malloc.h>

using namespace std;

#define N 10000

//文件太大了，改为随机生成
void readmv(/*string filename,*/ double **&matrix,double *&vector/*,int &n*/){
    // ifstream in(filename);
    //矩阵维度nxn
    // in >> n;
    matrix=(double **)malloc(N*sizeof(double*));
    for(int i=0;i<N;i++){
        matrix[i]=(double *)malloc(N*sizeof(double));
        for (int j=0;j<N;j++){
            // in>>matrix[i][j];
            matrix[i][j]=rand();
        }
    }
    vector=(double *)malloc(N*sizeof(double));
    for (int i=0;i<N;i++){
        // in>>vector[i];
        vector[i]=rand();
    }
    // in.close();
}

void gemm(const double *const *matrix,const double *vector_in,double *&vector_out,int n){
    vector_out=(double *)malloc(N*sizeof(double));
    for(int i=0;i<N;i++){
        vector_out[i]=0;
        for (int j=0;j<N;j++){
            vector_out[i]+=matrix[i][j]*vector_in[j];
        }
    }
}


int main(){
    double **matrix;
    double *vector_in,*vector_out;
    int n;
    string filename="sample.txt";
    clock_t t1=clock();
    readmv(/*filename,*/matrix,vector_in/*,n*/);
    clock_t t2=clock();
    gemm(matrix,vector_in,vector_out,n);
    clock_t t3=clock();
    // for(int i=0;i<n;i++){
    //     cout<<vector_out[i]<<" ";
    // }
    cout<<"\n"<<"readfile cost: "<<double(t2-t1)/CLOCKS_PER_SEC<<"s"<<endl;
    cout<<"caculate cost: "<<double(t3-t2)/CLOCKS_PER_SEC<<"s"<<endl;
}