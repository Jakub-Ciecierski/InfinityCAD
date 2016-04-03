//
// Created by jakub on 11/10/15.
//
/*
 * Since templates are the most stupid thing. The functions have to be defined
 * in the header file or explicit initializing of template for specific type
 * has to be included at the end of .cpp file. The latter approach has been
 * applied.
 *
 * http://stackoverflow.com/questions/8752837/undefined-reference-to-template-class-constructor
 */

#include "math/point.h"
#include <stdexcept>


//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

template <class T>
Point<T>::Point(){

}

template <class T>
Point<T>::Point(int size){
    elements = std::vector<T>(size);
}

template <class T>
Point<T>::Point(T* data, int count){
    for(int i = 0; i < count; i++){
        elements.push_back(data[i]);
    }
}

template <class T>
Point<T>::Point(const Point& p){
    elements = p.elements;
}


//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

/*
 * Add a dimension
 */
template <class T>
void Point<T>::addDimension(){
    elements.resize(this->size()+1);
}

template <class T>
void Point<T>::addDimensions(int count){
    for(int i = 0;i < count; i++){
        this->addDimension();
    }
}

template <class T>
void Point<T>::addDimension(T element){
    int size = this->size();

    elements.resize(this->size()+1);

    elements[size] = element;
}

/*
 * Returns the size of the point
 */
template <class T>
int Point<T>::size() const {
    return this->elements.size();
}

//-----------------------------------------------------------//
//  OPERATORS
//-----------------------------------------------------------//


template <class T>
T &Point<T>::operator[](int i) {
    int size = this->size();

    if (i >= 0 && i < size)
        return this->elements[i];
    else
        throw std::invalid_argument("Array out of bound");
}


template <class T>
const T &Point<T>::operator[](int i) const{
    int size = this->size();

    if (i >= 0 && i < size)
        return this->elements[i];
    else
        throw std::invalid_argument("Array out of bound");
}


template <class T>
Point<T> Point<T>::operator+(const Point<T>& p) const {
    int size1;
    int size2;

    size1 = this->size();
    size2 = p.size();

    if (size1 != size2){
        throw std::invalid_argument("The dimensions must match");
    }

    Point<T> newPoint(size1);

    for(int i = 0; i < size1; i++){
        newPoint[i] = (this->elements[i]) + p[i];
    }

    return newPoint;
}

template <class T>
void Point<T>::operator+=(const Point<T>& p) {
    int size1;
    int size2;

    size1 = this->size();
    size2 = p.size();

    if (size1 != size2){
        throw std::invalid_argument("The dimensions must match");
    }

    for(int i = 0; i < size1; i++){
        this->elements[i] = (this->elements[i]) + p[i];
    }
}

template <class T>
Point<T> Point<T>::operator+(const T& t) const{
    int size;

    size = this->size();

    Point<T> newPoint(size);

    for(int i = 0; i < size; i++){
        newPoint[i] = (this->elements[i]) + t;
    }

    return newPoint;
}

template <class T>
void Point<T>::operator+=(const T& t) {
    int size1;

    size1 = this->size();

    for(int i = 0; i < size1; i++){
        this->elements[i] = (this->elements[i]) + t;
    }
}

template <class T>
Point<T> Point<T>::operator-(const Point<T>& p) const{
    int size1;
    int size2;

    size1 = this->size();
    size2 = p.size();

    if (size1 != size2){
        throw std::invalid_argument("The dimensions must match");
    }

    Point<T> newPoint(size1);

    for(int i = 0; i < size1; i++){
        newPoint[i] = (this->elements[i]) - p[i];
    }

    return newPoint;
}

template <class T>
Point<T> Point<T>::operator/(const T& divider){

    int size = this->size();

    Point<T> newPoint(size);

    for(int i = 0; i < size; i++){
        newPoint[i] = (this->elements[i]) / divider;
    }

    return newPoint;
}

template <class T>
void Point<T>::operator/=(const T& divider){
    int size = this->size();

    for(int i = 0; i < size; i++){
        this->elements[i] = (this->elements[i]) / divider;
    }
}

template <class T>
bool Point<T>::operator==(const Point<T>& p) const{
    int size1;
    int size2;

    size1 = this->size();
    size2 = p.size();

    if (size1 != size2){
        throw std::invalid_argument("The dimensions must match");
    }

    for(int i = 0; i < size1; i++){
        if (this->elements[i] != p[i])
            return false;
    }
    return true;
}

template <class T>
bool Point<T>::operator!=(const Point<T>& p) const {
    int size1;
    int size2;
    int equalPart = 0;

    size1 = this->size();
    size2 = p.size();

    if (size1 != size2) {
        throw std::invalid_argument("The dimensions must match");
    }

    for (int i = 0; i < size1; i++) {
        if (this->elements[i] == p[i])
            equalPart++;
    }
    return (equalPart != size1);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Point<T>& p){
    os << "[";
    for(int i = 0;i < p.size(); i++) {
        os << p[i];
        if (i != p.size() - 1) {
            os << ", ";
        }
    }
    os << "]";

    return os;
}

template <class T>
Point<T> Point<T>::operator* (T x) const{
    Point<T> resultPoint;

    for(unsigned int dim = 0; dim < elements.size(); dim++) {
        resultPoint.addDimension(elements[dim] * x);
    }

    return resultPoint;
}

//-----------------------------------------------------------//
//  EXPLICIT INSTANTIATING
//-----------------------------------------------------------//

template class Point<char>;
template class Point<int>;
template class Point<float>;
template class Point<double>;

template std::ostream& operator<<(std::ostream& os, const Point<char>& p);
template std::ostream& operator<<(std::ostream& os, const Point<int>& p);
template std::ostream& operator<<(std::ostream& os, const Point<float>& p);
template std::ostream& operator<<(std::ostream& os, const Point<double>& p);
