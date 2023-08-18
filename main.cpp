#include <iostream>
#include <fstream>
#include <list>
#include <sstream>
#include <string.h>
//#include "r_star_tree.hpp"
#include "Actions.h"
#include "Actions.cpp"
#include <cstdio>
using namespace std;
/*
//Ένα πρόγραμμα C++ αρχίζει με τη main()
static std::string const INPUT_FILE_NAME ="BlockedMap.osm";
static std::string const COPY_FILE_NAME ="copyFile.osm";
static std::string const COULD_NOT_OPEN = "Could not open ";
*/
int main ()
{
    Actions x;
    x.add_entry("BlockedMap.osm","Amazing.osm");
    remove("copyFile.osm");
    /*
    ifstream inputFile;
    ofstream copyFile;
    string numOfBlocks;
    inputFile.open(INPUT_FILE_NAME);
    getline(inputFile,numOfBlocks);
    numOfBlocks=getSubstringBetween(numOfBlocks);
    //app so it doesn't overwrite the file
    copyFile.open(COPY_FILE_NAME,ios::app);
    if (!copyFile.is_open()){
        cout<< COULD_NOT_OPEN + COPY_FILE_NAME;
        return 1;
    }
    if (!inputFile.is_open()){
        cout<< COULD_NOT_OPEN + INPUT_FILE_NAME;
        return 1;
    }
    cout<<"hi"<<endl;
    string copyString;
    string findBlock="<block id=\"" + numOfBlocks + "\"";
    cout<<findBlock<<endl;
    while (getline(inputFile, copyString)) { 
        //Copying the data of a specific block
        if(copyString.compare(findBlock)==0){
            copyFile << copyString << "\n";
            while (getline(inputFile, copyString)){
                copyFile << copyString << "\n";
                if(copyString.compare("</block>")==0){
                    break;
                }
            }
            break;
        }
    }
    */
    /*
    else {
		char ch;

		while (!inputFile.eof()) {
			inputFile >> ch;
			outputFile << ch;
		}

	}
    */
   /*
    string copyString;
    ifstream copyFileRead;
    copyFileRead.open(COPY_FILE_NAME);
    while (getline(copyFileRead, copyString)) { 
        copyFile << copyString << "\n";
    }
	inputFile.close();
    copyFile.close();
    cout<<"\""<<endl;
    */
    return 0;
}/*
string copyString;
    ifstream copyFileRead;
    copyFileRead.open(COPY_FILE_NAME);
    while (getline(inputFile, copyString)) { 
        //Copying the data of a specific block
        if(copyString.compare("<block id=\"3\"")==0){
            copyFile << copyString << "\n";
            while (getline(inputFile, copyString)){
                copyFile << copyString << "\n";
                if(copyString.compare("</block>")==0){
                    break;
                }
            }
            break;
        }
    }
	inputFile.close();
    copyFile.close();
    */
