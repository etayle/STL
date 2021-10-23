#ifndef MAIN_CPP_AVLTREE_H
#define MAIN_CPP_AVLTREE_H
#include <iostream>
using namespace std;

#define BF 2
#define YES 1
#define NO 0
/* isFirstLarger - a function that needs to compare between two datas, if the first is larger than needs to
 * return positive, if equal return 0, and else return negative
 *
 * AVLtree() - constructor a object type AVLtree
 *
 * insert(T* data) - insert a data to tree
 *
 * remove(T* data) - delete the data *from the tree*
 *
 * removeWithData(T* data) - delete the data from the tree,and delete the data
 *                           the user need to support destructor to object
 *
 * findDo - find the data,and do on the data something, the user need to support class function
 *
 * inOrder - move on the tree at inOrder and on the data something,
 *           the user need to support class function
 */

/*
 * Generic AVLTree
 */
class NotFound: public exception{
    const char* what() const noexcept override
    {
        return "The data has not been found";
    }

};

class AlreadyExist: public exception{
    const char* what() const noexcept override
    {
        return "The data already exists";
    }

};



template <typename T, typename isFirstLarger>
class AVLTree {


public:
    class AVLNode {
    public:
        T* data;
        int size_of_right;
        int size_of_left;
        AVLNode *left;
        AVLNode *right;
        int traffic_at_right_son;
        int traffic_at_left_son;
        explicit AVLNode(T *data) : data(data), size_of_right(0), size_of_left(0), left(nullptr), right(nullptr)
                                ,traffic_at_right_son(0),traffic_at_left_son(0) {}
    };

    AVLNode *head;
    isFirstLarger compare;

    AVLTree() {
        head = nullptr;

    }
    ~AVLTree(){ this->postOrderDelete(this->head);

    }
    int size(){
        if(this->head== nullptr){
            return 0;
        }
        return this->head->size_of_left + this->head->size_of_right + 1;
    }
    /*
     * Does the right roll of the tree
     * Return: new root after the roll
     */
    AVLNode *rightRoll(AVLNode *node) {
        if (node == nullptr) {
            return nullptr;
        }
        if (node->left == nullptr) {
            return node;
        }
        AVLNode *save_left = node->left;  //A
        int node_to_add = save_left->size_of_right;
        int traffic_to_add = save_left->traffic_at_right_son;
        node->left = save_left->right;// BR=AR
        save_left->right = node;
        save_left->traffic_at_right_son = node->data->rank +
                                          node->traffic_at_right_son +
                                          save_left->traffic_at_right_son;
        save_left->size_of_right += 1 + node->size_of_right;
        node->size_of_left = node_to_add;
        node->traffic_at_left_son = traffic_to_add;
        return save_left;
    }

    /*
     * Does the left roll of the tree
     * Return: new root after the roll
     */
    AVLNode *leftRoll(AVLNode *node) {
        if (node == nullptr) {
            return nullptr;
        }
        if (node->right == nullptr) {
            return node;
        }
        AVLNode *save_right = node->right; // B
        int node_to_add = save_right->size_of_left;
        int traffic_to_add = save_right->traffic_at_left_son;
        node->right = save_right->left;
        save_right->size_of_left += 1 + node->size_of_left;
        save_right->traffic_at_left_son += node->data->rank +
                                           node->traffic_at_left_son;
        save_right->left = node;
        node->size_of_right = node_to_add;
        node->traffic_at_right_son = traffic_to_add;
        return save_right;
    }

    /*
     * Finds the relevant node with the relevant data and does something on it
     * FuncT - the class of the data that is passed
     */
    template<typename DoFunction, typename FuncT>
    void* findDo(FuncT* data, DoFunction& func) {
        return this->findDo<DoFunction>(this->head, data, func);
    };

    template<typename DoFunction, typename FuncT>
    void* findDo(AVLNode *node, FuncT* data, DoFunction& func) {
        if (node == nullptr) {
            throw NotFound();
        }
        if (!compare(data, node->data)) {
            return func(node->data);
        }
        if (compare(data, node->data) > 0) {
            return this->findDo<DoFunction>(node->right, data, func);
        } else {
            return this->findDo<DoFunction>(node->left, data, func);
        }
    }

    /*
    * insert function
    * Inserts a new node to the avl tree by the isFirstLarger generic function
    * Return:
    */
    void insert(T *data) {
        if (head == nullptr) {
            head = new AVLNode(data);
        } else {
            head = insert(head, data);
        }
    }

    AVLNode *insert(AVLNode *node, T *data) {
        if (compare(data, node->data)>0) {
            // We get here if we need to go right
            if (node->right == nullptr) {
                node->right = new AVLNode(data);
            } else {
                node->right = this->insert(node->right, data);
            }
            node->traffic_at_right_son += data->rank ;
            node->size_of_right++;
        } else if (compare(data, node->data)<0) {
            // We get here if we need to go left
            if (node->left == nullptr) {
                node->left = new AVLNode(data);

            } else {
                node->left = this->insert(node->left, data);
            }
            node->traffic_at_left_son += data->rank;
            node->size_of_left++;
        }
        else{
            throw AlreadyExist();
        }
        return this->balanceTree(node);
    }

    void remove(T *data) {
        if (head == nullptr) {
            return;
        } else {
            head = remove(head, data, NO);
        }
    }

    // remove the Vertice with the saved data
    void removeWithData(T *data) {
        if (head == nullptr) {
            return;
        } else {
            head = remove(head, data, YES);
        }
    }

    AVLNode *remove(AVLNode *node, T *data, int RemoveData) {
        //If not found in tree, the data we search to be deleted
        if (node == nullptr) { throw NotFound(); }
        //if find the data
        if (!compare(data, node->data)) {
            if (node->left == nullptr && node->right == nullptr) {
                if (RemoveData == YES) {
                    delete node->data;
                }
                delete node;
                return nullptr;
            }
            // the node have only one son
            else if (node->left == nullptr   ||   node->right == nullptr) {
                AVLNode *son;
                if (node->left == nullptr) {
                    son = node->right;
                } else {
                    son = node->left;
                }
                if (RemoveData == YES) {
                    delete node->data;
                }
                delete node;
                return son;
            } else {
                node = swap(node, this->getMinimum(node->right,data));
                node->traffic_at_right_son -= node->data->rank;
                node->right = this->remove(node->right, data, RemoveData);
                node->size_of_right--;
            }
            return this->balanceTree(node);
        }
        //if the data big from the the data at the current node,go right
        if (compare(data, node->data) > 0) {
            node->right = this->remove(node->right, data, RemoveData);
            node->size_of_right--;
            node->traffic_at_right_son -= data->rank;
        }
            //if the data small from the the data at the current node,go left
        else {
            node->left = this->remove(node->left, data, RemoveData);
            node->size_of_left--;
            node->traffic_at_left_son -= data->rank;
        }
        return this->balanceTree(node);
    }

    // does the DoFunction on the tree with inOrder
    template<typename DoFunction>
    void inOrder(DoFunction& func) {
        this->inOrder<DoFunction>(this->head, func);
    }

    template<typename DoFunction>
    void inOrder(AVLNode *node, DoFunction& func) {
        if (node == nullptr) {
            return;
        }
        this->inOrder<DoFunction>(node->left,func);
        func(node->data);
        this->inOrder<DoFunction>(node->right,func);

    }

    // Reversed in order, from right to left
    template<typename DoFunction>
    void reverseInOrder(DoFunction& func) {
        this->reverseInOrder<DoFunction>(this->head, func);
    }

    template<typename DoFunction>
    void reverseInOrder(AVLNode *node, DoFunction& func) {
        if (node == nullptr) {
            return;
        }
        this->reverseInOrder<DoFunction>(node->right, func);
        func(node->data);
        this->reverseInOrder<DoFunction>(node->left, func);
    }

    void postOrderDelete(AVLNode *node){
        if (node == nullptr) {
            return;
        }
        this->postOrderDelete(node->left);
        this->postOrderDelete(node->right);
        delete node;
    }
    AVLNode *getMinimum(AVLNode *node,T* data) {
        AVLNode* min_node;
        if (node->left == nullptr) {
            return node;
        } else {
            min_node = getMinimum(node->left,data);
            node->traffic_at_left_son-=min_node->data->rank ;
            node->traffic_at_left_son += data->rank;
        }
        return min_node;
    };

    AVLNode *swap(AVLNode *father, AVLNode *son) {
        T *temp_son_data = son->data;
        son->data = father->data;
        father->data = temp_son_data;
        return father;
    }

    AVLNode *balanceTree(AVLNode *node) {
        // The problem is on the right side

        if (this->get_balance_value(node) <= -BF) {
            // On the right side again - RR
            if (this->get_balance_value(node->right) <= 0) {
                // Only left roll
            }
                // On the left side - RL
            else {
                node->right = this->rightRoll(node->right);
            }
            node = leftRoll(node);
        }

            // The problem is on the left side
        else if (this->get_balance_value(node) >= BF) {
            // On the left side again - LL
            if (this->get_balance_value(node->left) >= 0) {
                // Only right roll
            }
                // On the right side - LR
            else {
                node->left = this->leftRoll(node->left);
            }
            node = rightRoll(node);
        }
        return node;
    }

    class treeToArray{
    public:
        T** array;
        int current_size;
        explicit treeToArray(int n):array(new T*[n]),current_size(0){}
        ~treeToArray(){
            delete[] array;
        }
        void operator()(T* obj){
            array[current_size] = obj;
            current_size++;
        }
    };

    void fillEmpty(T** array, int size){
        fillWithArray(head,array, 0, size-1);
    }

    void fillWithArray(AVLNode* node, T** array, int start, int end){
        if(node== nullptr) return;
        int mid = (end-start)/2+(end-start)%2+start;
        node->data = array[mid];
        fillWithArray(node->right, array, mid+1, end); // TODO: maybe it doesn't work with this indexes
        fillWithArray(node->left, array, start, mid-1);
        if(node->right != nullptr){
            node->traffic_at_right_son = node->right->data->rank +
                    node->right->traffic_at_right_son +
                    node->right->traffic_at_left_son;
        }
        if (node->left != nullptr){
            node->traffic_at_left_son = node->left->data->rank
                    + node->left->traffic_at_left_son
                    +node->left->traffic_at_right_son;
        }

    }

    AVLTree* merge(AVLTree* tree){
        int this_size = 0, tree_size = 0;
        if(head!= nullptr){
            this_size = head->size_of_left + head->size_of_right + 1;
        }
        if(tree->head!=nullptr){
            tree_size = tree->head->size_of_right + tree->head->size_of_left + 1;
        }
        int merged_size = tree_size + this_size;
        treeToArray this_array(this_size), tree_array(tree_size);

        // Getting the arrays from the trees
        this->inOrder(this_array);
        tree->inOrder(tree_array);
        T** merged_array = merge_sort(this_array.array, tree_array.array,this_size,tree_size);
        auto* merged_tree = emptyAVLTree(merged_size);
        merged_tree->fillEmpty(merged_array, merged_size);
        delete[] merged_array;
        return merged_tree;
    }

    T** merge_sort(T** a1, T** a2, int a1_size, int a2_size){
        T** merged = new T*[a1_size+a2_size];
        int a1_i=0, a2_i=0, merge_i=0;
        while(a1_i<a1_size&&a2_i<a2_size){
            if(compare(a1[a1_i], a2[a2_i])< 0){
                merged[merge_i] = a1[a1_i];
                a1_i++;
            }
            else{
                merged[merge_i] = a2[a2_i];
                a2_i++;
            }
            merge_i++;
        }
        while(a1_i<a1_size){
            merged[merge_i] = a1[a1_i];
            a1_i++;
            merge_i++;
        }
        while(a2_i<a2_size){
            merged[merge_i] = a2[a2_i];
            a2_i++;
            merge_i++;
        }
        return merged;
    }
    int SumHighestTrafficServers(int k){
        if(k<0){
            return -1;
        }
        if(this->head == nullptr){
            return 0;
        }
        if(this->size()<=k){
            return this->head->data->rank+
                    this->head->traffic_at_right_son +
                    this->head->traffic_at_left_son;
        }
        return this->SumHighestTrafficServers(this->head,k,0);
    }
    int SumHighestTrafficServers(AVLNode* node,int k,int sum){
        if(k==0){
            return sum;
        }
        else if (k == node->size_of_right +1 ) {
            return node->data->rank + node->traffic_at_right_son + sum;
        }
        else if( k == node->size_of_right){
            return node->traffic_at_right_son + sum;
        }
        else if(k < node->size_of_right){
            return this->SumHighestTrafficServers(node->right,k ,sum);
        }
        else{
            k-=(node->size_of_right+1);
            sum+= node->data->rank + node->traffic_at_right_son;
            return this->SumHighestTrafficServers(node->left,k ,sum);
        }
    }
private:
    int get_balance_value(AVLNode* node){
        return node->size_of_left - node->size_of_right;
    }

    AVLTree* emptyAVLTree(int size){
        auto* empty_tree = new AVLTree();
        empty_tree->head = emptyAVLNode(size);
        return empty_tree;
    }

    AVLNode* emptyAVLNode(int size){
        if (size==0) return nullptr;
        int sons_size = size-1;
        int left_size = int(sons_size/2)+(sons_size%2), right_size = sons_size - left_size;
        auto* empty_node = new AVLNode(nullptr);
        empty_node->size_of_right = right_size;
        empty_node->size_of_left = left_size;
        empty_node->left = emptyAVLNode(left_size);
        empty_node->right = emptyAVLNode(right_size);
    }
};




#endif //MAIN_CPP_AVLTREE_H
