#ifndef ACTIONS_H
#define ACTIONS_H
#include <string>
#include <stdlib.h>

#pragma once
using namespace std;
class Actions{
	public:
    Actions();
    void add_entry(string existingFile, string newFile);
};
#endif