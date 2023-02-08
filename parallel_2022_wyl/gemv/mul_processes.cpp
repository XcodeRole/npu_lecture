#include <iostream>
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <mpi.h>
#include <cmath>
// #include <string>
// #include <malloc.h>

/*
|A B C| |P| |AP+BQ+CK|
|D E F|x|Q|=|DP+EQ+FK|
|G H I| |K| |GP+HQ+IK|
*/

using namespace std;

//矩阵维度NxN
#define N 10000
// //进程数通过运行参数决定
// #define P 4

//全局参数，进程数以及进程编号
int world_size;
int pid;
MPI_Comm row_comm,col_comm;

//文件太大了，改为生成随机矩阵
void readmv(/*string filename,*/ double **&matrix,double *&vector,int dim){
    // ifstream in(filename);
    //矩阵维度nxn
    // in >> n;

    matrix=(double **)malloc(dim*sizeof(double*));
    for(int i=0;i<dim;i++){
        matrix[i]=(double *)malloc(dim*sizeof(double));
        for (int j=0;j<dim;j++){
            // in>>matrix[i][j];
            matrix[i][j]=rand();
        }
    }
    
    //为向量分配空间，如果是非对角线上的矩阵块，这块空间则作为接收缓存
    vector=(double *)malloc(dim*sizeof(double));

    //只有对角线上的进程初始化向量
    if ( pid/ int(sqrt(world_size)) == pid% int(sqrt(world_size)) ){
        for (int i=0;i<dim;i++){
            // in>>vector[i];
            vector[i]=rand();
        }
        
    }
    // cout<<pid<<" "<<pid % int(sqrt(world_size))<<endl;
    //对角线上的进程将向量广播到同一列的广播域中
    // MPI_Bcast(
    //     vector,
    //     dim,
    //     MPI_DOUBLE,
    //     pid % int(sqrt(world_size)), //对角线上的进程在col_comm通信域中的进程id
    //     col_comm
    // );
    
}

void gemm(const double *const *matrix,const double *vector_in,double *&vector_out,int dim){
    
    vector_out=(double *)malloc(N*sizeof(double));
    // for(int i=0;i<N;i++){
    //     vector_out[i]=0;
    //     for (int j=0;j<N;j++){
    //         vector_out[i]+=matrix[i][j]*vector_in[j];
    //     }
    // }
    
    for(int i=0;i<dim;i++){
        vector_out[i]=0;
        for (int j=0;j<dim;j++){
            vector_out[i]+=matrix[i][j]*vector_in[j];
        }
    }
    double *recv_buff=(double *)malloc(dim*sizeof(double));

    //将row_comm域上的向量reduce到对角线矩阵快对应进程中
    MPI_Reduce(
        vector_out,
        recv_buff,
        dim,
        MPI_DOUBLE,
        MPI_SUM,
        // pid/int(sqrt(world_size)), //对角线矩阵快对应进程在行通信域中的进程id
        int(sqrt(world_size))-1, //按照要求保存到最后一列
        row_comm
    );
    vector_out=recv_buff;
}


int main(){

    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);

    //每个进程中矩阵块大小为dim*dim
    int dim=N/sqrt(world_size);
    //划分行通信域
    MPI_Comm_split(MPI_COMM_WORLD,pid / int(sqrt(world_size)),pid,&row_comm);                     
    //划分列通信域
    MPI_Comm_split(MPI_COMM_WORLD,pid % int(sqrt(world_size)),pid,&col_comm);

    double **matrix;
    double *vector_in,*vector_out;

    // int n;
    // string filename="sample.txt";
    
    clock_t t1=clock();
    //从文件读入当前进程的数据matrix以及vector_in
    readmv(/*filename,*/matrix,vector_in,dim);

    clock_t t2=clock();
    gemm(matrix,vector_in,vector_out,dim);

    clock_t t3=clock();
    // for(int i=0;i<n;i++){
    //     cout<<vector_out[i]<<" ";
    // }
    cout<<"\n"<<"readfile cost: "<<double(t2-t1)/CLOCKS_PER_SEC<<"s"<<endl;
    cout<<"caculate cost: "<<double(t3-t2)/CLOCKS_PER_SEC<<"s"<<endl;

    MPI_Finalize();
}