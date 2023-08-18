#ifndef R_STAR_TREE_H
#define R_STAR_TREE_H
#include <string>
#include <stdlib.h>

#pragma once
using namespace std;
class Rectangle
{
public:
	string id;
	int num_of_child,area[2];
    bool leaf;
    Rectangle *children,*parent;
    Rectangle(){}
	Rectangle(int size,int top,int bottom,Rectangle *par)
	{
		id = "";
		num_of_child = 0;
		area[0]=bottom;
		area[1]=top;
		children=new Rectangle[size];
		parent=par;
	}
};
class Leaf{
	string id;
	int num_of_child,tree_height,area[2];
	Rectangle *parent;
	Leaf(int size,int top,int bottom, Rectangle *par){
		int children[size];
		id = "";
		num_of_child = 0;
		area[0]=bottom;
		area[1]=top;
		parent=par;
	}

};
class r_star_tree{
	public:
	r_star_tree();
	void add_rec(Rectangle *x);
	void add(Rectangle x, Rectangle y);
	Rectangle get(Rectangle x);
};
#endif