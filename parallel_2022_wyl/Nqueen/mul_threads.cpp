#include <iostream>
//频繁增删，使用list
#include<list>
#include<algorithm>
#include<omp.h>
#include <chrono>
using namespace std;
using namespace chrono;
#define N 14

int result=0;

// omp_lock_t listlock;
omp_lock_t resultlock;

void impl(list<int> &path,int cboard[]);
bool skip(list<int> &path,int next);
int main(){
    int cboard[N];
    for (int i=0;i<N;i++){
        cboard[i]=i;
    }
    list<int> path(0);
    // omp_init_lock(&listlock);
    auto t0=steady_clock::now();
    omp_init_lock(&resultlock);
    #pragma omp parallel default(shared)
    {
		#pragma omp single
		impl(path,cboard); 
    }
    auto t1=steady_clock::now();
    cout<<"computation time:"<<duration_cast<milliseconds>(t1-t0).count()<<"ms"<<endl;
    cout<<"共有"<<result<<"种解法"<<endl;
}   

//递归回溯，根据labudadong的模板改的
void impl(list<int> &path,int cboard[]){
	int n = path.size();
    if (n==N){
        omp_set_lock(&resultlock);
        result++;
        omp_unset_lock(&resultlock);
        return;
    }
    for (int i=0;i<N;i++){
        bool flag=skip(path,cboard[i]);
        if (flag)
            continue;
		path.push_back(cboard[i]);
        #pragma omp task shared(cboard)
        impl(path,cboard);
		path.pop_back();
    }
}

bool skip(list<int> &path,int next){
    int len=path.size();
    int i=0;
    for (auto iter=path.begin();iter!=path.end();iter++,i++){
        if (*iter==next || next==*iter+len-i || next == *iter-len+i)
            return true;
    }
    return false;
}