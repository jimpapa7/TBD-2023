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
	int num_of_child,tree_height;
    bool leaf;
    Rectangle *children;
    Rectangle(){}
	Rectangle(int size)
	{
		id = "";
		num_of_child = 0;
		children=new Rectangle[size];
	}
};
class r_star_tree{
	public:
	r_star_tree();
	void add_rec();
	void add(Rectangle x, Rectangle y);
	Rectangle get(Rectangle x);
};
#endif