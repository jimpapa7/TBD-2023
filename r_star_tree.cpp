#include "r_star_tree.h"
#include <iostream>
#include <cmath>
using namespace std;
//περνάει τις συντεταγμένες στο rectangle χρησιμοποιείται για καθαρισμό του κώδικα
void insert_lat_lon(Rectangle *x,int lat,int lon,bool check){
    if(check){
        if(x->minlon>lon){
            x->minlon=lon;
        }else if(x->maxlon<lon){
            x->maxlon=lon;
        }if(x->minlat>lat){
            x->minlat=lat;
        }else if(x->maxlat<lat){
            x->maxlat=lat;
        }
        return;
    }
    x->minlon=lon;
    x->maxlon=lon;
    x->minlat=lat;
    x->maxlat=lat;
}
//χρησιμοποιείται για να δώσουμε στα φύλλα που δεν μπήκε η εγγραφή ύψος ώστε το δέντρο να είναι ισοσταθμισμένο
void leaf_height_add(Rectangle x){
    if(x.isleaf){
        Rectangle z=x;
        x.isleaf=false;
        x.leaves.clear();
        x.children.insert ( x.children.begin() , z );
    }else{
        for(Rectangle cheker:x.children){
            leaf_height_add(cheker);
        }
    }
}
r_star_tree::r_star_tree(){
    root = NULL;
}
void r_star_tree::add_entry(int id,int lat,int lon,int size){
    bool flag=false;
    Node *newNode=new Node(id,lat,lon);
    //if the tree does not exist we create its root with the given key
    Rectangle *newRec=new Rectangle(size,lon,lat,lon,lat,NULL);
    if (root == NULL)
    {
        root = newRec;
        root->isleaf=true;
        root->leaves.insert ( root->leaves.begin() , *newNode );
        return;
    }
    Rectangle *tempRoot = NULL;
    Rectangle *tempChild = NULL;
    Rectangle *otherChild = NULL;
    Rectangle *temp = root;
    Rectangle *flagtemp=NULL;
    int templon,templat,distanceN,distanceO=100000000;
    int flagChild=0;
    while (temp != NULL && !flag) {
        for (Rectangle checker: temp->children) {
            //ελέγχει αν η θέση στην οποία βρίσκεται το στοιχείο είναι σε περιοχή κάθετα ή οριζόντια του rectangle
            if (checker.minlat <= lat) {
                if (checker.maxlat <= lat) {
                    templat = lat;
                } else {
                    templat = checker.maxlat;
                }
            } else {
                templat = checker.minlat;
            }
            if (checker.minlon <= lon) {
                if (checker.minlon <= lon) {
                    templat = lat;
                } else {
                    templat = checker.maxlon;
                }
            } else {
                templat = checker.minlon;
            }
            //ευκλείδεια απόσταση στου σημείου
            distanceN = sqrt((templon - lon) ^ 2 - (templat - lat) ^ 2);
            if (distanceN < distanceO) {
                //ελέγχει αν έχει περαστεί μία εγγραφή για να δει από ποιά μεριά πήγαμε στο δέντρο
                if (flagChild == 1) {
                    otherChild = tempChild;
                    flagChild = 2;
                }
                flagChild++;
                tempChild = &checker;
            }
        }
        if (tempChild->isleaf) {
            //ελέγχουμε αν χωράει η εγγραφή στο κλαδί
            if (tempChild->num_of_child < tempChild->size) {
                tempChild->leaves.insert(tempChild->leaves.begin(), *newNode);
                //ελέγχουμε αν μπορούμε να σπάσουμε το κλαδί
            } else if (temp->num_of_child < temp->size) {
                //σπάει το κλαδί σε δύο
                temp->num_of_child++;
                newRec->isleaf = true;
                newRec->leaves.insert(newRec->leaves.begin(), *newNode);
                temp->children.insert(temp->children.begin(), *newRec);
            } else {
                //δημιουργεί καινούργιο επίπεδο για κάδε κλαδί
                Rectangle *left, *right;
                left->isleaf = true;
                right->isleaf = true;
                left->size = size;
                right->size = size;
                left->num_of_child = ceil(size/2);
                right->num_of_child = floor(size/2);
                left->parent = tempChild;
                right->parent = tempChild;
                int i = 0;
                //δημιουργεί τα νέα κλαδιά με βάση τις lat lon συντεταγμένες τους
                for (Node checker: tempChild->leaves) {
                    i++;
                    if (i != 2) {
                        if (i == 0) {
                            insert_lat_lon(left, checker.lat, checker.lon, false);
                        } else {
                            insert_lat_lon(left, checker.lat, checker.lon, true);
                        }
                        left->leaves.insert(left->leaves.begin(), checker);
                    } else {
                        if (i == 2) {
                            insert_lat_lon(right, checker.lat, checker.lon, false);
                        } else {
                            insert_lat_lon(right, checker.lat, checker.lon, true);
                        }
                        right->leaves.insert(right->leaves.begin(), checker);
                    }
                }
                //αυξάνει το μέγεθος του δέντρου κατά ένα για να είναι όλα τα φύλλα στο ίδιο επίπεδο
                leaf_height_add(*otherChild);
                //θα πρέπει να γίνει και για την άλλη μεριά εκτός του δικού μας φύλλου έχει γίνει μόνο από το root στην άλλη πλευρά
            }
        // αν δεν είναι φύλλο συνεχίζει την πορεία του
        } else {
            temp = tempChild;
        }
    }
/*
 * Θα πηγαίνει και θα βάζει στο BlockedMap.osm την εγγραφή και θα παίρνει ένα id.
 * Θα παίρνει το block και το id.
 * Θα πηγαίνει στον indexer και θα κατατάσσει την εγγραφή με βάση το id.
 * Θα δημιουργεί ένα leaf και θα του δίνει το id.
 * Έπειτα θα δίνει στο rectangle το leaf στο leaves και θα ανανεώνει το rectangle σε isleaf=true.
 * Ίσως, να ελευθερώνει τα children από τη μνήμη αφού δε χρειάζονται και να τα δεσμεύει ξανά μετά αν χρειαστεί.
 * Το ίδιο να κάνει και με τα leaf αν έχει isleaf=false.
*/
}