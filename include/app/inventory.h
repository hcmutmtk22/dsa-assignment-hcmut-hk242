#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include "list/XArrayList.h"
#include "list/DLinkedList.h"
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace std;

// -------------------- List1D --------------------
template <typename T>
class List1D
{
public:
    class Iterator;

private:
    DLinkedList<T>* plist;

public:
    List1D();
    List1D(int num_elements);
    List1D(const T *array, int num_elements);
    List1D(const List1D<T> &other);
    virtual ~List1D();

    int size() const;
    T get(int index) const;
    void set(int index, T value);
    void add(const T &value);
    string toString() const;

    //added by me
    bool operator==(const List1D<T> &other);
    // List1D<T> operator=(List1D<T> &other);
    List1D<T> operator= (const List1D<T> &other);
    void removeAt(int index);
    void add(int index, const T &value) {plist->add(index, value);}

    friend ostream &operator<<(ostream &os, const List1D<T> &list){
        // TODO
        os << list.toString();
        return os;
    }

    Iterator begin() const {
        return Iterator(this, true);
    }

    Iterator end() const {
        return Iterator(this, false);
    }

    class Iterator {
    private:
        List1D<T>* dlist;
        typename DLinkedList<T>::Iterator it;

    public:
        Iterator() {
            dlist = NULL;
        }

        Iterator(const List1D<T>* dlist, bool begin = true) {
            this->dlist = new List1D<T>(*dlist);
            if (begin) {
                it = dlist->plist->begin();
            } else {
                it = dlist->plist->end();
            }
        }

        Iterator &operator=(const Iterator& other) {
            this->dlist = other.dlist;
            this->it = other.it;
            return (*this);
        }

        T operator*() {
            return *it;
        }
        
        bool operator!=(const Iterator& other) {
            return this->it != other.it;
        }

        Iterator &operator++() {
            ++it;
            return (*this);
        }

        Iterator operator++(int) {
            Iterator e = (*this);
            ++it;
            return e;
        }
    
    };
};

// -------------------- List2D --------------------
template <typename T>
class List2D
{
public:
    class Iterator;

private:
    XArrayList<List1D<T>> *pMatrix;

public:
    List2D();
    List2D(List1D<T> *array, int num_rows);
    List2D(const List2D<T> &other);
    virtual ~List2D();

    int rows() const;
    void setRow(int rowIndex, const List1D<T> &row);
    T get(int rowIndex, int colIndex) const;
    List1D<T> getRow(int rowIndex) const;
    string toString() const;

    //added
    void addRow(const List1D<T> &list);
    // List2D<T> operator= (List2D<T> &other);
    List2D<T> operator= (const List2D<T> &other);
    void removeRowAt(int index);

    friend ostream &operator<<(ostream &os, const List2D<T> &matrix)
    {
        // TODO
        os << matrix->toString();
        return os;
    }

    Iterator begin() const {
        return Iterator(this, true);
    }

    Iterator end() const {
        return Iterator(this, false);
    }

    // =============== ITERATOR OF LIST2D =============== 

    class Iterator {
    private:
        List2D<T>* dlist;
        typename XArrayList<List1D<T>>::Iterator it;

    public:
        Iterator() {
            dlist = NULL;
            
        }

        Iterator(const List2D<T>* dlist, bool begin = true) {
            this->dlist = new List2D<T>(*dlist);
            if (begin) {
                it = dlist->pMatrix->begin();
            } else {
                it = dlist->pMatrix->end();
            }
        }

        Iterator &operator=(const Iterator& other) {
            this->dlist = other.dlist;
            this->it = other.it;
            return (*this);
        }

        List1D<T> operator*() {
            return (*it);
        }
        
        bool operator!=(const Iterator& other) {
            return this->it != other.it;
        }

        void remove(void (*removeItemData)(T) = 0) {
            it.remove();
        }

        Iterator &operator++() {
            ++it;
            return (*this);
        }

        Iterator operator++(int) {
            it++;
            return (*this);
        }
    
    };
};

template <typename T>
string list1DToString(List1D<T>& list) {
    // return list.toString();

    stringstream ss;
    int size = list.size();
    ss << "[";
    // for (int i=0;i<size;i++) {
    //     ss << list.get(i);
    //     if (i<size-1) {
    //         ss << ", ";
    //     }
    // }
    int i=0;
    for (typename List1D<T>::Iterator it=list.begin(); it!=list.end(); it++) {
        ss << *it;
        if (i<size-1) {
            ss << ", ";
        }
        i++;
    }
    ss << "]";
    return ss.str();
}



struct InventoryAttribute
{
    string name;
    double value;
    InventoryAttribute() {
        name = "";
        value = 0;
    }
    InventoryAttribute(const string &name, double value) : name(name), value(value) {}
    string toString() const { return name + ": " + to_string(value); }

    InventoryAttribute &operator=(const InventoryAttribute &other) {
        this->name = other.name;
        this->value = other.value;
        return *this;
    }

    bool operator==(InventoryAttribute other) {
        return this->name==other.name && this->value==other.value;
    }

    bool operator!=(InventoryAttribute other) {
        return this->name!=other.name || this->value!=other.value;
    }

    string toString() {
        stringstream ss;
        ss << this->name << ", " << fixed << setprecision(6) << this->value;
        return ss.str();
    }

    friend ostream &operator<< (ostream& os, InventoryAttribute attribute) {
        os << attribute.name << ": " << fixed << setprecision(6) << attribute.value;
        return os;
    }
};

// -------------------- InventoryManager --------------------
class InventoryManager
{
private:
    List2D<InventoryAttribute> attributesMatrix;
    List1D<string> productNames;
    List1D<int> quantities;

public:
    InventoryManager();
    InventoryManager(const List2D<InventoryAttribute> &matrix,
                     const List1D<string> &names,
                     const List1D<int> &quantities);
    InventoryManager(const InventoryManager &other);

    int size() const;
    List1D<InventoryAttribute> getProductAttributes(int index) const;
    string getProductName(int index) const;
    int getProductQuantity(int index) const;
    void updateQuantity(int index, int newQuantity);
    void addProduct(const List1D<InventoryAttribute> &attributes, const string &name, int quantity);
    void removeProduct(int index);

    List1D<string> query(string attributeName, const double &minValue,
                         const double &maxValue, int minQuantity, bool ascending) const;

    void removeDuplicates();

    static InventoryManager merge(const InventoryManager &inv1,
                                  const InventoryManager &inv2);

    void split(InventoryManager &section1,
               InventoryManager &section2,
               double ratio) const;

    List2D<InventoryAttribute> getAttributesMatrix() const;
    List1D<string> getProductNames() const;
    List1D<int> getQuantities() const;
    string toString() const;
};

// -------------------- List1D Method Definitions --------------------
template <typename T>
List1D<T>::List1D()
{
    // TODO
    plist = new DLinkedList<T>();
}

template <typename T>
List1D<T>::List1D(int num_elements)
{
    // TODO
    plist = new DLinkedList<T>(0, 0, num_elements);
}

template <typename T>
List1D<T>::List1D(const T *array, int num_elements)
{
    // TODO
    plist = new DLinkedList<T>();
    for (int i=0;i<num_elements; i++) {
        plist->add(array[i]);
    }
}

template <typename T>
List1D<T>::List1D(const List1D<T> &other)
{
    // TODO
    this->plist = new DLinkedList<T>(*(other.plist));
}

template <typename T>
List1D<T>::~List1D()
{
    // TODO
    delete plist;
}

template <typename T>
int List1D<T>::size() const
{
    // TODO
    return plist->size();
}

template <typename T>
T List1D<T>::get(int index) const
{
    // TODO
    return plist->get(index);
}

template <typename T>
void List1D<T>::set(int index, T value)
{
    // TODO
    plist->set(index, value);
}

template <typename T>
void List1D<T>::add(const T &value)
{
    // TODO
    plist->add(value);
}

template <typename T>
string List1D<T>::toString() const
{
    // TODO
    return plist->toString();
}

template <typename T>
bool List1D<T>::operator==(const List1D<T> &other) {
    for (int i=0;i<size();i++) {
        if (get(i)!=other.get(i)) {
            return false;
        }
    }
    return true;
}

// template <typename T>
// List1D<T> List1D<T>::operator=(List1D<T> &other) {
//     this->plist = new XArrayList<T>(*(other.plist));
//     return (*this);
// }

template <typename T>
List1D<T> List1D<T>::operator= (const List1D<T> &other) {
    plist = new DLinkedList<T>(*(other.plist));
    return (*this);
}

template <typename T>
void List1D<T>::removeAt(int index) {
    plist->removeAt(index);
}

// template <typename T>
// ostream &operator<<(ostream &os, const List1D<T> &list)
// {
//     // TODO
//     os << list.toString();
//     return os;
// }

// -------------------- List2D Method Definitions --------------------
template <typename T>
List2D<T>::List2D()
{
    // TODO
    pMatrix = new XArrayList<List1D<T>>();
}

template <typename T>
List2D<T>::List2D(List1D<T> *array, int num_rows)
{
    // TODO
    pMatrix = new XArrayList<List1D<T>>();
    for (int i=0;i<num_rows;i++) {
        List1D<T> newlist = array[i];
        pMatrix->add(newlist);
    }
}

template <typename T>
List2D<T>::List2D(const List2D<T> &other)
{
    // TODO
    pMatrix = new XArrayList<List1D<T>>(*(other.pMatrix));
}

template <typename T>
List2D<T>::~List2D()
{
    // TODO
    delete pMatrix;
}

template <typename T>
int List2D<T>::rows() const
{
    // TODO
    return pMatrix->size();
}

template <typename T>
void List2D<T>::setRow(int rowIndex, const List1D<T> &row)
{
    // TODO
    // typename DLinkedList<List1D<T>>::Iterator it = pMatrix->begin();
    // for (int i=0;i<rowIndex;i++) {
    //     it++;
    // }
    // const List1D<T>& newrow = row;
    // *it = newrow;
    // cout << *it;
    // pMatrix->get(rowIndex) = newrow;

    pMatrix->set(rowIndex, row);
}

template <typename T>
T List2D<T>::get(int rowIndex, int colIndex) const
{
    // TODO
    List1D<T> row = pMatrix->get(rowIndex);
    return row.get(colIndex);
}

template <typename T>
List1D<T> List2D<T>::getRow(int rowIndex) const
{
    // TODO
    return pMatrix->get(rowIndex);
}

template <typename T>
string List2D<T>::toString() const
{
    // TODO
    return pMatrix->toString(&list1DToString);
}

template <typename T>
void List2D<T>::addRow(const List1D<T> &list) {
    pMatrix->add(list);
}

// template <typename T>
// List2D<T> List2D<T>::operator= (List2D<T> &other) {
//     this->pMatrix = new DLinkedList<List1D<T>>(*(other.pMatrix));
//     return (*this);
// }

template <typename T>
List2D<T> List2D<T>::operator= (const List2D<T> &other) {
    this->pMatrix = new XArrayList<List1D<T>>(*(other.pMatrix));
    return (*this);
}

template <typename T>
void List2D<T>::removeRowAt(int index) {
    pMatrix->removeAt(index);
}

// template <class T>
// ostream &operator<<(ostream &os, const List2D<T> &matrix)
// {
//     // TODO
//     os << matrix->toString();
//     return os;
// }

// -------------------- InventoryManager Method Definitions --------------------
InventoryManager::InventoryManager()
{
    // TODO
    List2D<InventoryAttribute> newattributes;
    List1D<string> newnames;
    List1D<int> newquantities;
    this->attributesMatrix = newattributes;
    this->productNames = newnames;
    this->quantities = newquantities;
}

InventoryManager::InventoryManager(const List2D<InventoryAttribute> &matrix,
                                   const List1D<string> &names,
                                   const List1D<int> &quantities)
{
    // TODO
    this->attributesMatrix = matrix;
    this->productNames = names;
    this->quantities = quantities;
}

InventoryManager::InventoryManager(const InventoryManager &other)
{
    // TODO
    this->attributesMatrix = other.attributesMatrix;
    this->productNames = other.productNames;
    this->quantities = other.quantities;
}

int InventoryManager::size() const
{
    // TODO
    return quantities.size();
}

List1D<InventoryAttribute> InventoryManager::getProductAttributes(int index) const
{
    // TODO
    return attributesMatrix.getRow(index);
}

string InventoryManager::getProductName(int index) const
{
    // TODO
    return productNames.get(index);
}

int InventoryManager::getProductQuantity(int index) const
{
    // TODO
    return quantities.get(index);
}

void InventoryManager::updateQuantity(int index, int newQuantity)
{
    // TODO
    return quantities.set(index, newQuantity);
}

void InventoryManager::addProduct(const List1D<InventoryAttribute> &attributes, const string &name, int quantity)
{
    // TODO
    attributesMatrix.addRow(attributes);
    productNames.add(name);
    quantities.add(quantity);
}

void InventoryManager::removeProduct(int index)
{
    // TODO
    attributesMatrix.removeRowAt(index);
    productNames.removeAt(index);
    quantities.removeAt(index);
}

List1D<string> InventoryManager::query(string attributeName, const double &minValue,
                                       const double &maxValue, int minQuantity, bool ascending) const
{
    // TODO
    const List2D<InventoryAttribute>* p = &attributesMatrix;
    List2D<InventoryAttribute>::Iterator it(p);
    List1D<string> result;
    List1D<double> value;
    List1D<int> qtt;

    int i=0;
    for (it; it!=p->end(); it++) {
        List1D<InventoryAttribute> row = *it;
        for (int j=0; j<row.size(); j++) {
            InventoryAttribute attribute = row.get(j);
            if (attribute.name == attributeName && attribute.value >= minValue 
                && attribute.value <= maxValue && quantities.get(i) >= minQuantity) {
                if (ascending) {
                    // SORT ASCENDING
                    if (value.size()>0) {
                        int k=0;
                        while ( k<value.size() && value.get(k)<attribute.value) {
                            k++;
                        }
                        if (k<value.size() && value.get(k)==attribute.value && qtt.get(k)<=quantities.get(i)) {
                            // compare 2 quantities
                            result.add(k+1, productNames.get(i));
                            value.add(k+1, attribute.value);
                            qtt.add(k+1, quantities.get(i));
                        } else {
                            result.add(k, productNames.get(i));
                            value.add(k, attribute.value);
                            qtt.add(k, quantities.get(i));
                        }
                    } else {
                        result.add(productNames.get(i));
                        value.add(attribute.value);
                        qtt.add(quantities.get(i));
                        break;
                    }
                }
                else {
                    // result.add(productNames.get(i));
                    // break;

                    //SORT DESCENDING
                    if (value.size()>0) {
                        int k=0;
                        while (k<value.size() && value.get(k)>attribute.value) {
                            k++;
                        }
                        if (k<value.size() && value.get(k)==attribute.value && qtt.get(k)>quantities.get(i)) {
                            // compare 2 quantities
                            result.add(k+1, productNames.get(i));
                            value.add(k+1, attribute.value);
                            qtt.add(k+1, quantities.get(i));
                        } else {
                            result.add(k, productNames.get(i));
                            value.add(k, attribute.value);
                            qtt.add(k, quantities.get(i));
                        }
                    } else {
                        result.add(productNames.get(i));
                        value.add(attribute.value);
                        qtt.add(quantities.get(i));
                        break;
                    }
                }
            }
        }
        i++;
    }
    return result;
}

void InventoryManager::removeDuplicates()
{
    // TODO
    List2D<InventoryAttribute>::Iterator it(&attributesMatrix);
    int i=0;
    for (it; it!=(&attributesMatrix)->end();it++) {
        List1D<InventoryAttribute> row = *it;
        List2D<InventoryAttribute>::Iterator jt = it;
        jt++;
        int j=i+1;
        for (jt; jt!=(&attributesMatrix)->end(); ++jt) {
            if (*it == *jt) {
                int quantity = quantities.get(j);
                quantities.set(i, quantities.get(i)+ quantity);
                // attributesMatrix.removeRowAt(j);
                jt.remove();
                productNames.removeAt(j);
                quantities.removeAt(j);
            }
            j++;
        }
        i++;
    }
}

InventoryManager InventoryManager::merge(const InventoryManager &inv1,
                                         const InventoryManager &inv2)
{
    // TODO
    InventoryManager inv(inv1);
    List2D<InventoryAttribute>::Iterator it(&inv2.attributesMatrix);
    int i=0;
    for (it; it!=(&inv2.attributesMatrix)->end(); it++) {
        inv.addProduct(*it, inv2.productNames.get(i), inv2.quantities.get(i));
        i++;
    }
    return inv;
}

void InventoryManager::split(InventoryManager &section1,
                             InventoryManager &section2,
                             double ratio) const
{
    // TODO
    int i=0;
    double n = ceil(ratio * productNames.size());
    int num_first = (int) n;
    List2D<InventoryAttribute>::Iterator it(&attributesMatrix, true);
    for (it; i<num_first;it++, i++) { // adding sublist
        section1.addProduct(*it, productNames.get(i), quantities.get(i));
    }
    for (it; i<productNames.size(); it++, i++) {
        section2.addProduct(*it, productNames.get(i), quantities.get(i));
    }
}

List2D<InventoryAttribute> InventoryManager::getAttributesMatrix() const
{
    // TODO
    return this->attributesMatrix;
}

List1D<string> InventoryManager::getProductNames() const
{
    // TODO
    return this->productNames;
}

List1D<int> InventoryManager::getQuantities() const
{
    // TODO
    return this->quantities;
}

string InventoryManager::toString() const
{
    // TODO
    stringstream ss;
    ss << "InventoryManager[" << endl
        << "  AttributesMatrix: " << attributesMatrix.toString() << "," << endl
        << "  ProductNames: " << productNames.toString() << "," << endl
        << "  Quantities: " << quantities.toString() << endl
        << "]";
    return ss.str();
}

#endif /* INVENTORY_MANAGER_H */
