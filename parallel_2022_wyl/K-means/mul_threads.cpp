#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <set>
#include <cmath>
#include <limits.h>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace chrono;
#define K 16


//从filename文件中读取矩阵到matrix中，matrix的维度是row_dim*col_dim
void readmatrix(string filename,double **&matrix,int &row_dim,int &col_dim){
    ifstream in;
    in.open(filename.c_str());
    //读入行和列
    in>>row_dim>>col_dim;
    matrix=(double **)malloc(row_dim*sizeof(double *));
    for(int i=0;i<row_dim;i++){
        matrix[i]=(double *)malloc(col_dim*sizeof(double));
        for (int j=0;j<col_dim;j++){
            in>>matrix[i][j];
        }
    }
    in.close();
}

//计算两个长度为n的向量X,Y的欧式距离
double distance(const double *X,const double *Y,int n){
    double sum=0;
    for (int i=0;i<n;i++){
        sum+=pow(fabs(X[i]-Y[i]),2);
    }
    return sqrt(sum);

}

//计算最小误差SSE
void getSSE(const double *const* matrix,const int row_dim,const int col_dim,const double * const*center,const set<int> cluster[K],double &sse){
    #pragma omp for reduction(+:sse)
    for (int i=0;i<K;i++){
        for (auto iter=cluster[i].begin();iter!=cluster[i].end();iter++){
            sse+=pow(distance(center[i],matrix[*iter],col_dim),2);
        }
    }
}

//根据距离重新划分聚类
void updateCluster(const double *const* matrix,const int row_dim,const int col_dim,const double *const* center,set<int> cluster[K]){
    //清空聚类容器
    // #pragma omp barrier
    #pragma omp single
    for(int i=0;i<K;i++){
        cluster[i].clear();
    }
    // 这里single字句会隐式同步，所以不需要barrier
    // #pragma omp barrier
    //遍历所有样本点，也就是矩阵matrix的行向量，重新划分聚类
    #pragma omp for
    for(int i=0;i<row_dim;i++){
        double min_dis=INFINITY;
        //最近cluster中心的id
        int cluster_id;
        //遍历聚类中心，也就是center的行向量，求最小距离的聚类id
        for(int j=0;j<K;j++){
            double dis=distance(matrix[i],center[j],col_dim);
            if (dis<min_dis){
                min_dis=dis;
                cluster_id=j;
            }
        }
        // cout<<cluster_id<<" "<<i<<endl;
        //将第i个坐标归入第cluster_id个聚类
        #pragma omp critical
        cluster[cluster_id].insert(i);
    }
}


//通过更新的聚类更新聚类中心点坐标
void updateCenter(const double *const*matrix,const int row_dim,const int col_dim,double **center,set<int> cluster[K]){
    
    // double *center_index=(double*)malloc(col_dim*sizeof(double));
    //遍历K个聚类，重新计算center
    #pragma omp for
    for(int i=0;i<K;i++){
        for (auto iter=cluster[i].begin();iter!=cluster[i].end();iter++){
            for(int j=0;j<col_dim;j++){
                center[i][j]+=matrix[*iter][j];
            }
        }
        for(int j=0;j<col_dim;j++){
            center[i][j]/=cluster[i].size();
        }
    }
}


//矩阵每一行代表一个col_dim维的向量
void k_means(const double *const* matrix,const int row_dim,const int col_dim){
    srand(0);
    //K个聚类
    double **center=(double **)malloc(K*sizeof(double*));
    //初始化，选前K个向量为聚类中心
    for (int i=0;i<K;i++){
        center[i]=(double *)malloc(col_dim*sizeof(double));
        for (int j=0;j<col_dim;j++){
            center[i][j]=matrix[i][j];
        }
    }

    //存储K个聚类，每个聚类是一个由矩阵行id构成的set
    set<int> cluster[K];
    //均匀划分初始化每个聚类
    for (int i=0;i<K;i++){
        for(int j=0;j<row_dim/K;j++){
            cluster[i].insert(i+(j*K));
        }
    }
    double pre_sse=0;
    getSSE(matrix,row_dim,col_dim,center,cluster,pre_sse);
    double delta_sse;
    //核心迭代，直到收敛
    do{
        double sse=0;
        #pragma omp parallel default(shared)
        {
            updateCluster(matrix,row_dim,col_dim,center,cluster);
            updateCenter(matrix,row_dim,col_dim,center,cluster);
            getSSE(matrix,row_dim,col_dim,center,cluster,sse);
            #pragma omp single
            {
                delta_sse=fabs(sse-pre_sse);
                // cout<<"delta_sse="<<delta_sse<<endl;
                pre_sse=sse;
            }
        }
    }while(delta_sse>1e-8);


    for (int i=0;i<K;i++){
        cout<<"cluster "<<i<<": ";
        for (auto iter=cluster[i].begin();iter!=cluster[i].end();iter++){
            cout<<*iter<<" ";
        }
        cout<<endl;
    }
}

int main(){
    double **matrix;
    int row_dim,col_dim;
    string filename="samples.data";

    //读取数据矩阵，行，列
    readmatrix(filename,matrix,row_dim,col_dim);
    auto t0=steady_clock::now();
    k_means(matrix,row_dim,col_dim);
    auto t1=steady_clock::now();
    cout<<"computation time:"<<duration_cast<milliseconds>(t1-t0).count()<<"ms"<<endl;
}