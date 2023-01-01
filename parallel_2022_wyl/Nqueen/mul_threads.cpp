#include <iostream>
//频繁增删，使用list
#include<list>
#include<algorithm>
#include<omp.h>
using namespace std;
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
    omp_init_lock(&resultlock);
    #pragma omp parallel default(shared) num_threads(2)
    {
		#pragma omp single
		impl(path,cboard); 
    }
    cout<<result<<endl;
}   

//递归回溯，按照labudadong的模板写的
void impl(list<int> &path,int cboard[]){
	int n;
	// omp_set_lock(&listlock);
	n = path.size();
    // omp_unset_lock(&listlock);
    if (n==N){
        omp_set_lock(&resultlock);
        result++;
        omp_unset_lock(&resultlock);
        return;
    }
    for (int i=0;i<N;i++){
        // omp_set_lock(&listlock);
        bool flag=skip(path,cboard[i]);
        // omp_unset_lock(&listlock);
        if (flag)
            continue;
        // omp_set_lock(&listlock);
		path.push_back(cboard[i]);
        // omp_unset_lock(&listlock);
        #pragma omp task
        impl(path,cboard);
		// omp_set_lock(&listlock);
		path.pop_back();
        // omp_unset_lock(&listlock);
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