#include <iostream>
#include <string.h>
class Super;
typedef void(Super::*func)();
class Base{
protected:
    int test;
    Super* supes;
    func _func;
public:

    Base(int addr){
        std::cout<<addr<<"\n";
    }
    void tryThis(Super* s){
        supes = s;
        (*supes.*_func)();
    }
};
class Super: public Base{
public:
    Super():Base((int)(this)){
        std::cout<<(int)(this)<<" "<<this<<"\n";
        _func = &Super::Greet;
        Base::tryThis(this);
    }
     void Greet(){
        std::cout<<"HELLO WORLD \n";
    }
};
int main(int argc, char* argv[]){
    //Super s = Super();
    char ten[11];
    //memset(ten, 0x00, 11);    
    for(int i =0; i < 10; i++){
        (ten[i]) = '+'; 
    }
    char* eight = &(ten[2]);
    //s.Greet();
    return 0;
}
