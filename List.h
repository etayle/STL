#ifndef HW0_LIST_H
#define HW0_LIST_H


#include <bits/exception.h>

template<typename T>
class List{
    int size;

public:
    class Node{
    public:
        T& data;
        Node* next;
        Node* prev;

        explicit Node(T& data, Node* next= nullptr, Node* prev= nullptr): data(data), next(next), prev(prev){}

    };

    Node* head;
    Node* tail;

    List():head(nullptr), tail(nullptr), size(0){};
    ~List(){
        while(head != nullptr){
            Node* to_delete = head;
            head = head->next;
            delete to_delete;
        }
    }

    /*
     * Inserts the data in the end of the list
     * Return: Node that was created
     */
    Node* insertFirst(T& data){
        Node* insertion = new Node(data, head);
        if(head == nullptr){
            head = insertion;
            tail = insertion;
        }
        else{
            head->prev = insertion;
            head = insertion;
        }
        size++;
        return insertion;
    }

    /*
     * Inserts the data in the end of the list
     * Return: Node that was created
     */
    Node* insertLast(T& data){
        Node *insertion = new Node(data, nullptr, tail);
        if(tail == nullptr){
            head = insertion;
            tail = insertion;
        }
        else{
            tail->next = insertion;
            tail = insertion;
        }
        size++;
        return insertion;
    }

    /*
     * Deletes the first node in the list and returns it
     * Return: Node
     */
    Node* popFirst(){
        if (head == nullptr || this->size==0){
            return nullptr;
        }
        Node* pop_head = head;
        head = head->next;
        if (head == nullptr){
            tail = head;
        }
        size--;
        return pop_head;
    }

    /*
     * Deletes the last node in the list and returns it
     * Return: Node
     */
    Node* popEnd(){
        if (tail == nullptr){
            return nullptr;
        }
        Node* pop_tail = tail;
        tail = tail->prev;
        if(tail== nullptr){
            head = tail;
        }
        size--;
        return pop_tail;
    }

    /*
     * Removes the node from the list, does not delete the node itself
     * Please note that the node needs to be in the list
     */
    void detach(Node* node){
        if(node == nullptr){
            return;
        }

        if(node == head){
            head = head->next;
            if (head == nullptr){
                tail = head;
            }
        }
        else if(node==tail){
            tail = tail->prev;
            if(tail == nullptr){
                head = tail;
            }
        }
        else{
            node->next->prev = node->prev;
            node->prev->next = node->next;
        }
        node->next = nullptr;
        node->prev = nullptr;
        size--;
    }

    Node* getFirst(){
        return head;
    }

    Node* getLast(){
        return tail;
    }

    // Sets the head and tail to be null
    Node* clear(){
        head = nullptr;
        tail = nullptr;
    }
};

#endif //HW0_LIST_H
