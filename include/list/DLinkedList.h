/*
 * File:   DLinkedList.h
 */

#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include "list/IList.h"

#include <sstream>
#include <iostream>
#include <type_traits>
using namespace std;

template <class T>
class DLinkedList : public IList<T>
{
public:
    class Node;        // Forward declaration
    class Iterator;    // Forward declaration
    class BWDIterator; // Forward declaration

protected:
    Node *head; // this node does not contain user's data
    Node *tail; // this node does not contain user's data
    int count;
    bool (*itemEqual)(T &lhs, T &rhs);        // function pointer: test if two items (type: T&) are equal or not
    void (*deleteUserData)(DLinkedList<T> *); // function pointer: be called to remove items (if they are pointer type)

public:
    DLinkedList(
        void (*deleteUserData)(DLinkedList<T> *) = 0,
        bool (*itemEqual)(T &, T &) = 0);
    DLinkedList(const DLinkedList<T> &list);
    DLinkedList<T> &operator=(const DLinkedList<T> &list);
    ~DLinkedList();

    // Inherit from IList: BEGIN
    void add(T e);
    void add(int index, T e);
    T removeAt(int index);
    bool removeItem(T item, void (*removeItemData)(T) = 0);
    bool empty();
    int size();
    void clear();
    T &get(int index);
    int indexOf(T item);
    bool contains(T item);
    string toString(string (*item2str)(T &) = 0);
    // Inherit from IList: END

    void set(int index, T item);

    void println(string (*item2str)(T &) = 0)
    {
        cout << toString(item2str) << endl;
    }
    void setDeleteUserDataPtr(void (*deleteUserData)(DLinkedList<T> *) = 0)
    {
        this->deleteUserData = deleteUserData;
    }

    bool contains(T array[], int size)
    {
        int idx = 0;
        for (DLinkedList<T>::Iterator it = begin(); it != end(); it++)
        {
            if (!equals(*it, array[idx++], this->itemEqual))
                return false;
        }
        return true;
    }

    /*
     * free(DLinkedList<T> *list):
     *  + to remove user's data (type T, must be a pointer type, e.g.: int*, Point*)
     *  + if users want a DLinkedList removing their data,
     *      he/she must pass "free" to constructor of DLinkedList
     *      Example:
     *      DLinkedList<T> list(&DLinkedList<T>::free);
     */
    static void free(DLinkedList<T> *list)
    {
        typename DLinkedList<T>::Iterator it = list->begin();
        while (it != list->end())
        {
            delete *it;
            it++;
        }
    }

    /* begin, end and Iterator helps user to traverse a list forwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::Iterator it;
     for(it = list.begin(); it != list.end(); it++){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    Iterator begin()
    {
        return Iterator(this, true);
    }
    Iterator end()
    {
        return Iterator(this, false);
    }

    /* last, beforeFirst and BWDIterator helps user to traverse a list backwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::BWDIterator it;
     for(it = list.last(); it != list.beforeFirst(); it--){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    BWDIterator bbegin()
    {
        return BWDIterator(this, true);
    }
    BWDIterator bend()
    {
        return BWDIterator(this, false);
    }

protected:
    static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &))
    {
        if (itemEqual == 0)
            return lhs == rhs;
        else
            return itemEqual(lhs, rhs);
    }
    void copyFrom(const DLinkedList<T> &list);
    void removeInternalData();
    Node *getPreviousNodeOf(int index);

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
public:
    class Node
    {
    public:
        T data;
        Node *next;
        Node *prev;
        friend class DLinkedList<T>;

    public:
        Node(Node *next = 0, Node *prev = 0)
        {
            this->next = next;
            this->prev = prev;
        }
        Node(T data, Node *next = 0, Node *prev = 0)
        {
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };

    //////////////////////////////////////////////////////////////////////
    class Iterator
    {
    private:
        DLinkedList<T> *pList;
        Node *pNode;

    public:
        Iterator(DLinkedList<T> *pList = 0, bool begin = true)
        {
            if (begin)
            {
                if (pList != 0)
                    this->pNode = pList->head->next;
                else
                    pNode = 0;
            }
            else
            {
                if (pList != 0)
                    this->pNode = pList->tail;
                else
                    pNode = 0;
            }
            this->pList = pList;
        }

        Iterator &operator=(const Iterator &iterator)
        {
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node *pNext = pNode->prev; // MUST prev, so iterator++ will go to end
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }

        T &operator*()
        {
            return pNode->data;
        }
        bool operator!=(const Iterator &iterator)
        {
            return pNode != iterator.pNode;
        }
        // Prefix ++ overload
        Iterator &operator++()
        {
            pNode = pNode->next;
            return *this;
        }
        // Postfix ++ overload
        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
    };

    // Backward Iterator
    class BWDIterator
    {
    private:
        DLinkedList<T> *pList;
        Node *pNode;

    public:
        BWDIterator(DLinkedList<T> *pList = 0, bool last = true)
        {
            if (last)
            {
                if (pList != 0)
                    this->pNode = pList->tail->prev;
                else
                    pNode = 0;
            }
            else
            {
                if (pList != 0)
                    this->pNode = pList->head;
                else
                    pNode = 0;
            }
            this->pList = pList;
        }

        BWDIterator &operator=(const BWDIterator &iterator)
        {
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node *pNext = pNode->next; // MUST next, so iterator-- will go to head
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }

        T &operator*()
        {
            return pNode->data;
        }
        bool operator!=(const BWDIterator &iterator)
        {
            return pNode != iterator.pNode;
        }
        // Prefix -- overload
        BWDIterator &operator--()
        {
            pNode = pNode->prev;
            return *this;
        }
        // Postfix -- overload
        BWDIterator operator--(int)
        {
            BWDIterator iterator = *this;
            --*this;
            return iterator;
        }
    };
};
//////////////////////////////////////////////////////////////////////
// Define a shorter name for DLinkedList:

template <class T>
using List = DLinkedList<T>;

//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
DLinkedList<T>::DLinkedList(
    void (*deleteUserData)(DLinkedList<T> *),
    bool (*itemEqual)(T &, T &))
{
    // TODO
    this->deleteUserData = deleteUserData;
    this->itemEqual = itemEqual;
    head = new Node();
    tail = new Node();
    head->next = tail;
    tail->prev = head;
    this->count = 0;
}

template <class T>
DLinkedList<T>::DLinkedList(const DLinkedList<T> &list)
{
    // TODO

    itemEqual = list.itemEqual;
    deleteUserData = list.deleteUserData;

    head = new Node();
    tail = new Node();
    head->next = tail;
    tail->prev = head;
    count = 0;
    Node* p = list.head->next;
    while (p!=list.tail) {
        add(p->data);
        p = p->next;
    }
}

template <class T>
DLinkedList<T> &DLinkedList<T>::operator=(const DLinkedList<T> &list)
{
    // TODO
    head = new Node();
    tail = new Node();
    head->next = tail;
    tail->prev = head;
    count = 0;
    Node* p = list.head->next;
    while (p!=list.tail) {
        add(p->data);
        p = p->next;
    }
    return (*this);
}

template <class T>
DLinkedList<T>::~DLinkedList()
{
    // TODO
    Node* p = head;
    while (p) {
        Node* temp = p;
        p = p->next;
        delete temp;
    }
}

template <class T>
void DLinkedList<T>::add(T e)
{
    // TODO
    // if (count==0) {
    //     head->data = e;
    //     count++;
    //     return;
    //     // Node* n = new Node(e);
    //     // n->next = tail;
    //     // n->prev = head;
    //     // head->next = n;
    //     // tail->prev = n;
    //     // count++;
    //     // return;
    // }
    Node* p = new Node(e);
    // tail->next = p;
    // p->prev = tail;
    // tail = tail->next;
    p->prev = tail->prev;
    p->next = tail;
    tail->prev->next = p;
    tail->prev = p;
    count++;
}
template <class T>
void DLinkedList<T>::add(int index, T e)
{
    // TODO
    if (index>count || index<0) throw std::out_of_range("Sorry, the index is invalid");
    if (index==count || count==0) {
        add(e);
        return;
    }
    Node* p = new Node(e);
    if (index==0) {
        p->next = head;
        head->prev = p;
        head = p;
    } else {
        Node* curr = head;
        for (int i=0;i<index;i++) {
            curr = curr->next;
        }
        p->next = curr;
        p->prev = curr->prev;
        curr->prev->next = p;
        curr->prev = p;
    }
    count++;
}

template <class T>
typename DLinkedList<T>::Node *DLinkedList<T>::getPreviousNodeOf(int index)
{
    /**
     * Returns the node preceding the specified index in the doubly linked list.
     * If the index is in the first half of the list, it traverses from the head; otherwise, it traverses from the tail.
     * Efficiently navigates to the node by choosing the shorter path based on the index's position.
     */
    // TODO
    if (index<0 || index>=count) throw std::out_of_range("The index is not valid");
    if (index<count/2) {
        Node* curr = head->next;
        Node* previous = head;
        for (int i=0;i<index;i++) {
            previous = curr;
            curr = curr->next;
        }
        return previous;
    }
    if (index>=count/2) {
        Node* curr = tail->prev;
        Node* previous = curr->prev;
        for (int i=count-1;i>index;i--) {
            if (previous) previous = previous->prev;
            curr = curr->prev;
        }
        return previous;
    }
}

template <class T>
T DLinkedList<T>::removeAt(int index)
{
    // TODO
    if (index<0 || index>=count) {
        throw std::out_of_range("The index is not valid");
    }
    count--;
    // if (index==0) {
    //     T result = head->next->data;
    //     Node* p = head;
    //     if (head->next) {
    //         head = head->next;
    //         head->prev = NULL;
    //     }
    //     delete p;
    //     return result;
    // }
    // if (index==count-1) {
    //     T result = tail->data;
    //     Node* p = tail;
    //     tail = tail->prev;
    //     tail->next = NULL;
    //     delete p;
    //     return result;
    // }
    Node* p = head->next;
    for (int i=0;i<index;i++) {
        p = p->next;
    }
    T result = p->data;
    p->prev->next = p->next;
    p->next->prev = p->prev;
    delete p;
    return result;
}

template <class T>
bool DLinkedList<T>::empty()
{
    // TODO
    return this->count==0;
}

template <class T>
int DLinkedList<T>::size()
{
    // TODO
    return count;
}

template <class T>
void DLinkedList<T>::clear()
{
    // TODO
    int t = this->count;
    for (int i=0;i<t;i++) {
        removeAt(0);
    }
}

template <class T>
T &DLinkedList<T>::get(int index)
{
    // TODO
    if (index<0 || index>=count) {
        throw std::out_of_range("The index is not valid");
    }
    if (index<count/2) {
        Node* p = head->next;
        for (int i=0;i<index;i++) {
            p = p->next;
        }
        return p->data;
    }
    else {
        Node* p = tail->prev;
        for (int i=count-1;i>index;i--) {
            p = p->prev;
        }
        return p->data;
    }
}

template <class T>
int DLinkedList<T>::indexOf(T item)
{
    // TODO
    Node* p = head->next;
    int index = 0;
    while (p!=tail) {
        if (equals(p->data, item, itemEqual)) {
            return index;
        }
        index++;
        p = p->next;
    }
    return -1;  
}

template <class T>
bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T))
{
    // TODO
    if (removeItemData != NULL) {
        removeItemData(item);
    }
    int index = indexOf(item);
    if (index==-1) return false;
    removeAt(index);
    return true;
}

template <class T>
bool DLinkedList<T>::contains(T item)
{
    // TODO
    return indexOf(item)!=-1;
}

template <class T>
void DLinkedList<T>::set(int index, T item) {
    Node* p = head->next;
    for (int i=0;i<index;i++) {
        p = p->next;
    }
    p->data = item;
}

template <class T>
string DLinkedList<T>::toString(string (*item2str)(T &))
{
    /**
     * Converts the list into a string representation, where each element is formatted using a user-provided function.
     * If no custom function is provided, it directly uses the element's default string representation.
     * Example: If the list contains {1, 2, 3} and the provided function formats integers, calling toString would return "[1, 2, 3]".
     *
     * @param item2str A function that converts an item of type T to a string. If null, default to string conversion of T.
     * @return A string representation of the list with elements separated by commas and enclosed in square brackets.
     */
    // TODO
    stringstream ss;
    ss << "[";
    Node* p = head->next;
    while (p!=tail) {
        if (item2str) ss << item2str(p->data);
        else {
            ss << p->data;
        }
        if (p->next!=tail) ss << ", ";
        p = p->next;
    }
    ss << "]";
    return ss.str();
}

template <class T>
void DLinkedList<T>::copyFrom(const DLinkedList<T> &list)
{
    /**
     * Copies the contents of another doubly linked list into this list.
     * Initializes the current list to an empty state and then duplicates all data and pointers from the source list.
     * Iterates through the source list and adds each element, preserving the order of the nodes.
     */
    // TODO
    head = tail = new Node();
    Iterator it = list.begin();
    for (it=list.begin(); it!=list.end();it++) {
        add(*it);
    }
}

template <class T>
void DLinkedList<T>::removeInternalData()
{
    /**
     * Clears the internal data of the list by deleting all nodes and user-defined data.
     * If a custom deletion function is provided, it is used to free the user's data stored in the nodes.
     * Traverses and deletes each node between the head and tail to release memory.
     */
    // TODO
    clear();
    delete itemEqual;
    delete deleteUserData;
}

#endif /* DLINKEDLIST_H */
