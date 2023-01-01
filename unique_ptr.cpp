#include <iostream>
#include <memory>

using namespace std;

class Test {
    public:
    int a;
    Test(int a) {
        this->a = a;
        cout<<"Inside constructor of Test"<< endl;
    }
    
    ~Test(){
        cout<<"Inside destructor of Test"<< endl;
    }
};

void func(std::unique_ptr<Test> &test)
{
    cout<<"Inside func. test.a= "<<test->a<< endl;
}

void func_unique(std::unique_ptr<Test> test)
{
    cout<<"Inside func_unique. test.a= "<<test->a<< endl;
}

int main()
{
    cout<<"Hello World"<<endl;
    auto test_obj = make_unique<Test>(5);
    
    //func_unique(std::move(test_obj));
      func(test_obj);
    cout<<"Back to main "<<endl;

    return 0;
}
