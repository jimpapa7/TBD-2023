#include "Actions.h"
#include <iostream>
#include <fstream>
#include <list>
#include <sstream>
#include <cstdio>
#include <string>
#include <sys/stat.h>
#include <future>
//Διαιρέσαμε τις λειτουργίες σε δύο διαφορετικές κλάσεις για να μπορούμε να δουλέψουμε ασύγχρονα και να περιμένουμε το αποτέλεσμα
using namespace std;
static std::string const COPY_FILE_NAME ="copyFile.osm";
static std::string const minlon ="26.4812000";
static std::string const maxlon ="41.5322000";
static std::string const minlat ="41.4438000";
static std::string const maxlat ="26.5952000";
//επιστρέφει τον αριθμό των blocks που αναφέρονται στο πρώτο block
string getSubstringBetween(std::string input){
    std::string startStr="blocks=\"";
    std::string endStr="\"";
    std::size_t startPos=input.find(startStr);
    startPos+=startStr.length();
    std::size_t endPos=input.find(endStr,startPos);
    return input.substr(startPos,endPos-startPos);
}
//ελέγχει αν το τελευταίο block χωράει την εισερχόμενη εγγραφή
int copyFileCheck(string existingFile,string newFile){
    ifstream oldFileR,newFileR;
    ofstream copyFile;
    string numOfBlocks;
    oldFileR.open(existingFile);
    copyFile.open("copyFile.osm",ios::app);
    newFileR.open(newFile);
    getline(oldFileR,numOfBlocks);
    numOfBlocks=getSubstringBetween(numOfBlocks);
    string copyString;
    ifstream copyFileRead;
    string findBlock="<block id=\"" + numOfBlocks + "\"";
    int flagwait=0;
    while (getline(newFileR, copyString)) {
        copyFile << copyString << "\n";
    }
    while (getline(oldFileR, copyString)) {
        //Copying the data of a specific block
        if(copyString.compare(findBlock)==0){
            copyFile << copyString << "\n";
            while (getline(oldFileR, copyString)){
                copyFile << copyString << "\n";
                if(copyString.compare("</block>")==0){
                    break;
                }
            }
            break;
        }
    }
    return 2;
}
//η κλάση χρησιμοποιείται για να περιμένουμε την ολοκλήρωση της αντιγραφής
int updateFile(string existingFile,string newFile,bool overUnder) {
    ifstream oldFileR, newFileR;
    ofstream copyFile;
    copyFile.open(COPY_FILE_NAME);
    newFileR.open(newFile);
    oldFileR.open(existingFile, ios::app);
    string id;
    bool flag = true;
    string numOfBlocks;
    getline(oldFileR,numOfBlocks);
    numOfBlocks=getSubstringBetween(numOfBlocks);
    string deleteline = "</block>",copyString;
    string findBlock="<block id=\"" + numOfBlocks + "\"";
    int pos;
    //ελέγχει αν το αρχείο είναι πάνω από το επιτρεπόμενο όριο
    if (overUnder) {
        while (getline(oldFileR, copyString)) {
            //Βρίσκει το τελευταίο μπλοκ
            /*
            if(pos!=-1){
               id = copyString.substr(0,pos+1);
            }
            */
            pos = copyString.find('</block>');
            id = copyString.substr(0, pos + 1);
            //Δεν αντιγράφει το </block> που είναι στο τελευταίο block που θα μπει η εγγραφή
            if (deleteline.compare(id) != 0 || flag) {
                copyFile << copyString + "\n";
                if (copyString.compare(findBlock) == 0) {
                    flag = false;
                }
            }
        }
        while (getline(newFileR, copyString)) {
            copyFile << copyString << "\n";
        }
        copyFile << "</block>";
    } else {
        stringstream ss, ss2;
        int numB;
        ss << numOfBlocks;
        ss >> numB;
        numB++;
        ss2 << numB;
        ss2 >> numOfBlocks;
        //Δημιουργεί ένα νέο αρχείο με το νέο μπλοκ και αυξάνει τον αριθμό των blocks
        copyFile << "block id=\"0\" blocks=\"" + numOfBlocks + " </block>\n";
        while (getline(oldFileR, copyString)) {
            copyFile << copyString << "\n";
        }
        copyFile << "<block id=\"" + numOfBlocks + "\"";
        copyFile << "\n";
        while (getline(newFileR, copyString)) {
            copyFile << copyString << "\n";
        }
        copyFile << "</block>";
        copyFile.close();
    }
    return 2;
}
Actions::Actions(){}
//εισάγει τη νέα εγγραφή
void Actions::add_entry(string existingFile, string newFile){
    ifstream oldFileR,newFileR;
    ofstream copyFile;
    oldFileR.open(existingFile);
    //app so it doesn't overwrite the file
    copyFile.open("copyFile.osm",ios::app);
    newFileR.open(newFile);
    if (!oldFileR.is_open()){
        cout<< "BlockedMap.osm"<<endl;
        return ;
    }
    if (!copyFile.is_open()){
        cout<<existingFile;
    }
    if (!newFileR.is_open()){
        cout<<newFile<<endl;
    }
    std::future<int> myFuture = std::async(&copyFileCheck,"BlockedMap.osm","Amazing.osm");
    //καθυστερεί μέχρι την ολοκλήρωση του γραψίματος του νέου αρχείου
    int result=myFuture.get();
    struct stat file_status;
    stat("copyFile.osm",&file_status);
    copyFile.close();
    newFileR.close();
    oldFileR.close();
    int result2;
    //checking if the new block will be over 32kb
    if(file_status.st_size<32000){
        myFuture = std::async(&updateFile,existingFile,newFile,true);
        result=myFuture.get();
        remove("BlockedMap.osm");
        rename("copyFile.osm","BlockedMap.osm");
        return;
    }
    myFuture = std::async(&updateFile,existingFile,newFile,false);
    result=myFuture.get();
    remove("BlockedMap.osm");
    rename("copyFile.osm","BlockedMap.osm");
}
//διαγράφει μία εγγραφή με βάση το id της και το block στο οποίο βρίσκεται
//λειτουργεί ως η ασύγχρονη κλάση για να περιμένουμε την αντιγραφή του αρχείου πριν να του αλλάξουμε όνομα
int copyDelete(string block,string id) {
    ifstream oldFileR;
    ofstream copyFile;
    oldFileR.open("BlockedMap.osm");
    //app so it doesn't overwrite the file
    copyFile.open(COPY_FILE_NAME, ios::app);
    string copyString;
    bool flagBlock = true, flagNode = true;
    string deleteBlock = "<block id=\"" + block + "\"";
    int pos;
    size_t found;
    int i = 0;
    string node;
    //για να πάρει ακριβώς το string
    int length = 11 + id.length();
    id = " <node id=\"" + id;
    while (getline(oldFileR, copyString)) {
        i++;
        //Βρίσκει το τελευταίο μπλοκ
        /*
        if(pos!=-1){
           id = copyString.substr(0,pos+1);
        }
        */
        pos = copyString.find(id);
        node = copyString.substr(0, length);
        //Βρίσκει το block στο οποίο βρίσκεται η εγγραφή
        found = copyString.find(id);
        if (copyString.compare(deleteBlock) == 0) {
            flagBlock = false;
        }
        //Αν είναι στο σωστό block βρίσκει το id της εγγραφής
        if (!flagBlock && id.compare(node) == 0) {
            flagNode = false;
        }
        //Μόλις τελείωσει η εγγραφή συνεχίζει ακυρώνει τα flag για να αντιγράψει τα υπόλοιπα δεδομένα
        if (copyString.compare(" </node>") == 0 && !flagBlock && !flagNode) {
            flagNode = true;
            flagBlock = true;
        }
        if (flagBlock || flagNode) {
            copyFile << copyString + "\n";
        }
    }
    return 2;
}
//διαγράφει μία εγγραφή με βάση το id της και το block στο οποίο βρίσκεται
void Actions::delete_entry(string block, string id) {
    std::future<int>myFuture = std::async(&copyDelete,block,id);
    int result=myFuture.get();
    remove("BlockedMap.osm");
    rename("copyFile.osm","BlockedMap.osm");
}