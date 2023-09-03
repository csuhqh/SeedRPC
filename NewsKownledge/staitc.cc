#include <iostream>

class A{
    public:
        static int getId(){
            ++a;
            return a;
        }

    private:
        static int a;
};
int A::a = 0; //类外初始化

int main(){
    
    std::cout << A::getId() << std::endl;

    return 0;
}