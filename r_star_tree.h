#ifndef R_STAR_TREE_H
#define R_STAR_TREE_H
#include <string>
#include <stdlib.h>
#include <vector>
#pragma once
using namespace std;
class Node{
public:
    int id,lat,lon;
    Node(){
        id=NULL;
    }
    Node(int id,int lat,int lon){
    this->id=id;
    this->lon=lon;
    this->lat=lat;
    }
};
class Rectangle
{
public:
	string id;
	int num_of_child,minlon,minlat,maxlon,maxlat,size;
    Rectangle *parent;
    std::vector<Rectangle> children;
    std::vector<Node> leaves;
    bool isleaf;
    Rectangle(){}
	Rectangle(int size,int bottomlon,int bottomlat,int toplon,int toplat,Rectangle *par)
	{
        this->size=size;
		id = "";
		num_of_child = 0;
        minlon=bottomlon;
		minlat=bottomlat;
		maxlon=toplon;
		maxlat=toplat;
		children={};
        isleaf=false;
        leaves= {};
		parent=par;
	}
};
class r_star_tree{
    Rectangle *root;
	public:
	r_star_tree();
	void add_rec(int size,int top,int bottomlon,int bottomlat,int toplon,int toplat);
    void add_entry(int id,int lat,int lon,int size);
};
#endif