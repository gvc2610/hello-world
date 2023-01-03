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

//void test_func(unique_ptr<Test> func_ptr)
void test_func(unique_ptr<Test> &func_ptr)

{
    cout << "Inside test_func "<< func_ptr->a <<endl;
    func_ptr->a = 15;
}

unique_ptr<Test> create_unique(int val)
//void create_unique(int val)

{
    auto ptr = make_unique<Test>(val);
    ptr->a = 10;
    test_func(ptr);
    return ptr;
}

// int main()
// {
//     cout<<"Hello World\n";
//     unique_ptr ptr_main = ::create_unique(5);
//     cout << ptr_main->a<<endl;

//     return 0;
// }

int main()
{
    cout<<"Hello World"<<endl;
    auto test_obj = make_unique<Test>(5);
    
    //func_unique(std::move(test_obj));
      func(test_obj);
    cout<<"Back to main "<<endl;

    return 0;
}
