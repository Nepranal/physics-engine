#include <iostream>
#include <thread>
#include<time.h>
#include<chrono>
#include <unistd.h>


using namespace std;

int main(){
    while(1){
        sleep(1);
        cout<<"Hello?"<<endl;
    }
}