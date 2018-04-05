#include <iostream>
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
    Super s = Super();
    //s.Greet();
    return 0;
}
