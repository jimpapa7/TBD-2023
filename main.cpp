#include <iostream>
#include "r_star_tree.hpp"
using namespace std;
//Ένα πρόγραμμα C++ αρχίζει με τη main()
int main ()
{
    Rectangle *x=new Rectangle(2);
    Rectangle *y=new Rectangle(1);
    Rectangle *z=new Rectangle(3);
    x->children[0]=*y;
    cout<<x->children[0].id<<endl;
    return 0;
}
