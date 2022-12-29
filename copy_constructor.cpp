#include <iostream>

using namespace std;

class Line {
   public:
      int getLength( void );
      Line( int len );             // simple constructor
      Line( const Line &obj);  // copy constructor
      ~Line();                     // destructor
      int * getPtr(){ return ptr;}

   private:
      int *ptr;
};

// Member functions definitions including constructor
Line::Line(int len) {
   cout << "Normal constructor allocating ptr" << endl;
   
   // allocate memory for the pointer;
   ptr = new int;
   *ptr = len;
   cout << hex<< ptr << endl;
}

Line::Line(const Line &obj) {
   cout << "Copy constructor allocating ptr." << endl;
   ptr = new int;
   *ptr = *obj.ptr; // copy the value
}

Line::~Line(void) {
   cout << "Freeing memory!" << endl;
   delete ptr;
}

int Line::getLength( void ) {
   return *ptr;
}

void display(Line obj) {
   cout << "Length of line : " << obj.getLength() <<endl;
   cout << "display obj ptr  "<< hex<< obj.getPtr()<< endl;
}

void display_test(Line &obj) {
   cout << "display_test Length of line : " << obj.getLength() <<endl;
   cout << "display_test obj ptr  "<< hex<< obj.getPtr()<< endl; 
}

// Main function for the program
int main() {

   Line line1(10);

   Line line2 = line1; // This also calls copy constructor

   display(line1);
   display(line2);

   cout << "line1  " << hex << line1.getPtr()<<endl;
   cout << "line2  " << hex << line2.getPtr()<<endl;
   
   display_test(line1);

   return 0;
}