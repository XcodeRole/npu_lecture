#include <iostream>
//频繁增删，使用list
#include<list>
#include<algorithm>
#include <chrono>
using namespace std;
using namespace chrono;

//控制 N 的值
#define N 14

//解的个数
int result=0;

void impl(list<int> &path,int cboard[]);
bool skip(list<int> &path,int next);
int main(){
    int cboard[N];
    for (int i=0;i<N;i++){
        cboard[i]=i;
    }
    list<int> path(0);
    auto t0=steady_clock::now();
    impl(path,cboard);
    auto t1=steady_clock::now();
    cout<<"computation time:"<<duration_cast<milliseconds>(t1-t0).count()<<"ms"<<endl;
    cout<<"共有"<<result<<"种解法"<<endl;
}   

//递归回溯，根据labudadong的模板改的
void impl(list<int> &path,int cboard[]){
    if (path.size()==N){
        result++;
        return;
    }
    for (int i=0;i<N;i++){
        if (skip(path,cboard[i]))
            continue;
        path.push_back(cboard[i]);
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