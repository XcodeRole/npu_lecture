#include <list>ls
#include <algorithm>
#include <iostream>
using namespace std;

void exec(list<int> &path,int num[]);

const int N = 4;
list<list<int>> result;

int main(){
    int num[]={0,1,2,3};
    list<int> path(0);
    exec(path,num);
    int count=0;
    for (auto i=result.begin();i!=result.end();i++){
        for (auto j=i->begin();j!=i->end();j++){
            cout<<*j;
        }
        count++;
        cout<<endl;
    }
    cout<<count<<endl;
}
void exec(list<int> &path,int num[]){
    if (path.size()==N){
        result.push_back(path);
        return;
    }
    for(int i=0;i<N;i++){
        if (find(path.begin(),path.end(),num[i])!=path.end()){
            continue;
        }
        path.push_back(num[i]);
        exec(path,num);
        path.pop_back();
    }
}