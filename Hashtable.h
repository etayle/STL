#ifndef HW2_HASHTABLE_H
#define HW2_HASHTABLE_H

#include <cmath>

#include "AVLTree.h"
#define START_SIZE 3
#define HASH_MULTIPLIER (sqrt(5)-1)/2
using namespace std;



// getNumFunc - function that extracts the reference number from the T object
template<typename T, typename getNumFunc>
class Hashtable {
    T** hashtable;
    bool* occupied;
    int max_size;
    int current_size;
    int highest;
    double a;
    getNumFunc get_num_func;

    // Modolu
    int h1(int num){
        return num % max_size;
    }

    // Complex hash function
    int h2(int num, int k){
        double multi_num = a*num;
        double frac = multi_num - int(multi_num);
        return int(k*max_size*frac);
    }

public:
    Hashtable():hashtable(new T*[START_SIZE]), occupied(new bool[START_SIZE]), max_size(START_SIZE), current_size(0),
    a(HASH_MULTIPLIER), highest(0){
        for(int i=0;i<START_SIZE;i++){
            occupied[i] = false;
        }
    }
    ~Hashtable(){
        delete[] hashtable;
        delete[] occupied;
    }
    // Inserts the object into the hashtable using the hash function
    void insert(T* obj) {
            if (current_size == max_size) {
                try {
                    this->find_index(get_num_func(obj));
                    throw AlreadyExist();
                }
                catch (NotFound &e) {}
                // Need to multiply the table size
                int new_size = max_size * 2;
                bool *new_occupied = new bool[new_size];
                for (int i = 0; i < new_size; i++) {
                    new_occupied[i] = false;
                }
                T **new_hashtable = new T *[new_size];
                T **temp_old_hashtable = hashtable;
                bool *temp_old_occupied = occupied;
                hashtable = new_hashtable;
                int old_size = max_size;
                max_size = new_size;
                occupied = new_occupied;
                current_size = 0;
                highest = 0;
                for (int i = 0; i < old_size; i++) {
                    if(temp_old_occupied[i]){
                        this->insert(temp_old_hashtable[i]);
                    }
                }
                delete[] temp_old_occupied;
                delete[] temp_old_hashtable;
            }

            for (int k = 0; true; k++) {
                int obj_num = get_num_func(obj);
                // Double hashing
                int index = (h1(obj_num) + h2(obj_num, k)) % max_size;
                if (!occupied[index]) {
                    occupied[index] = true;
                    hashtable[index] = obj;
                    current_size++;
                    if (index > highest) {
                        highest = index;
                    }
                    break;
                } else {
                    if (get_num_func(hashtable[index]) == obj_num) {
                        throw AlreadyExist();
                    }
                }
            }
        }

    // Finds the object with the relevant number and return the index in the hashtable
    int find_index(int num){
        int num_of_already_checked=0;
        bool visited[this->max_size] = {false};
        for(int k=0;true;k++) {
            // Double hashing
            int index = (h1(num)+h2(num, k))%max_size;
            if (occupied[index]) {
                T* temp_obj = hashtable[index];
                if(get_num_func(temp_obj)==num){
                    return index;
                }
                if(!visited[index]) {
                    num_of_already_checked++;
                }
            }
            visited[index] = true;
            if(num_of_already_checked == current_size){
                throw NotFound();
            }
        }

    }

    T* find(int num){
        return hashtable[find_index(num)];
    }

    T* remove(int num){
        int index = find_index(num);
        hashtable[index] = nullptr;
        occupied[index] = false;
        current_size--;
        if (index==highest){
            for(int i=highest-1;i>=0;i--){
                if(occupied[i]){
                    highest=i;
                    break;
                }
            }
        }
        if(highest<max_size/2&&max_size>START_SIZE){ // TODO: maybe we need here <=
            int new_size = max_size / 2;
            bool* new_occupied = new bool[new_size];
            for (int i = 0; i < new_size; i++) {
                new_occupied[i] = false;
            }
            T** new_hashtable = new T*[new_size];
            T **temp_old_hashtable = hashtable;
            bool *temp_old_occupied = occupied;
            hashtable = new_hashtable;
            max_size = new_size;
            occupied = new_occupied;
            current_size = 0;
            highest = 0;
            for (int i = 0; i < new_size; i++) {
                if(temp_old_occupied[i]){
                    this->insert(temp_old_hashtable[i]);
                }
            }
            delete[] temp_old_occupied;
            delete[] temp_old_hashtable;
        }
    }

    // Deletes all the data inside the hashtable
    void delete_all_data(){
        for(int i=0;i<max_size;i++){
            if(occupied[i]){
                delete hashtable[i];
            }
        }
    }
};

#endif //HW2_HASHTABLE_H
