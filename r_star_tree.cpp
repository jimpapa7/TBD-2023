#include "r_star_tree.hpp"
#include <iostream>
using namespace std;

r_star_tree::r_star_tree(){

}
void r_star_tree::add_rec(){
    Rectangle *newRectangle = new Rectangle(2);
}
void r_star_tree::add(Rectangle x, Rectangle y){
    x.children[0]=y;
}
Rectangle r_star_tree::get(Rectangle x){
    return x.children[0];
}