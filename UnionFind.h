//
// Created by etayle on 27/12/2019.
//

#ifndef HW2_UNIONFIND_H
#define HW2_UNIONFIND_H

/////////////////////////////////Union_Find
//// Makeset(i)  - make a set with 1 element with ID  i
// find(i)       - find the ID of the set to which the element belongs
// unite         - union the set of elemet p belongs with the set of elemet q belongs
using namespace std;
class InvalidInput: public exception{
    const char* what() const noexcept override
    {
        return "The input has not valid";
    }

};
class UnionFind {
public:
    class UnionNode {
    public:
        int node_num;
        int size_of_set;
        UnionNode *parent;
        explicit UnionNode(int i) : node_num(i), size_of_set(1), parent(nullptr) {}
    };
    int max_size;
    UnionNode** array;
    void makeSet(int i){
        array[i] = new UnionNode(i);
    }
    explicit UnionFind(int n): max_size(n){
        array = new  UnionNode *[n + 1];
       for(int i = 0;i<n+1;i++){
           this->makeSet(i);
       }
    }
    ~UnionFind(){
        for(int i =0;i<max_size+1;i++){
            delete array[i];
        }
        delete[] array;
    }
    int find(int i){
        if (i<1 || this->max_size<i){
            throw InvalidInput();
        }
        return getParent(this->array[i]);
    }
    int getParent(UnionNode* node){
        if (node->parent == nullptr){
            return node->node_num;
        }
        else {
            int index = getParent(node->parent);
            node->parent =  this->array[index];
            return index;
        }
    }
    int unite(int p, int q){
        if(p<0||q<0|| this->max_size<p||this->max_size<q){
            throw InvalidInput();
        }
        p = this->find(p);
        q = this->find(q);
        if (p==q){
            return p;
        }
        if(array[p]->size_of_set<array[q]->size_of_set) {
            array[p]->parent = array[q];
            array[q]->size_of_set += array[p]->size_of_set;
            return q;
        }
        else {
            array[q]->parent = array[p];
            array[p]->size_of_set += array[q]->size_of_set;
            return p;
        }
    }
};
#endif //HW2_UNIONFIND_H