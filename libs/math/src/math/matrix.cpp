//
// Created by jakub on 12/3/15.
//

#include <stdexcept>
#include <math/matrix.h>
/*
template <class T>
Matrix<T>::Matrix(unsigned int n, unsigned int m) : n(n), m(m)
{
    // n rows
    entries.resize(n);

    for(unsigned int i = 0; i < n; i++){
        entries[i].resize(m);
    }
}

template <class T>
Matrix<T>::Matrix(unsigned int n, unsigned int m, T val){
    entries.resize(n);

    for(unsigned int i = 0; i < n; i++){
        entries[i].resize(m);
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            entries[i][j] = val;
        }
    }
}

template <class T>
Matrix<T>::Matrix(const Matrix& matrix) : n(matrix.n), m(matrix.m)
{
    this->entries = matrix.entries;
}


//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

template <class T>
const std::vector<T>& Matrix<T>::getColumn(int i) const{
    if(i < 0 || i >= m)
        throw std::invalid_argument("Array out of Bound. Matrix::setEntry");

    std::vector<T> column(n);
    for(int j = 0; j < n; j++){
        column[j] = entries[i][j];
    }
    return column;
}

template <class T>
void Matrix<T>::setColumn(int i, const std::vector<T>& vec){
    if(i < 0 || i >= m)
        throw std::invalid_argument("Array out of Bound. Matrix::setEntry");

    std::vector<T> column(n);
    for(int j = 0; j < n; j++){
        entries[i][j] = vec[j];
    }
}

template <class T>
void Matrix<T>::setEntry(unsigned int i, unsigned int j, const T& value){
    if(i < 0 || i >= n || j < 0 || j >= m)
        throw std::invalid_argument("Array out of Bound. Matrix::setEntry");

    this->entries[i][j] = value;
}

template <class T>
const T& Matrix<T>::getEntry(unsigned int i,unsigned int j) const{
    if(i < 0 || i >= n || j < 0 || j >= m)
        throw std::invalid_argument("Array out of Bound. Matrix::getEntry");

    return this->entries[i][j];
}

template <class T>
unsigned int Matrix<T>::rowCount() const{
    return this->n;
}

template <class T>
unsigned int Matrix<T>::columnCount() const{
    return this->m;
}

//-----------------------------------------------------------//
//  OPERATORS
//-----------------------------------------------------------//

template <class T>
std::vector<T>& Matrix<T>::operator[](unsigned int i) {
    if(i < 0 || i >= n)
        throw std::invalid_argument("Array out of Bound. Matrix::operator[]");

    return this->entries[i];
}

template <class T>
const std::vector<T>& Matrix<T>::operator[](unsigned int i) const{
    if(i < 0 || i >= n)
        throw std::invalid_argument("Array out of Bound. Matrix::operator[]");

    return this->entries[i];
}


template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix){

    unsigned int n = matrix.rowCount();
    unsigned int m = matrix.columnCount();

    for(unsigned int i = 0; i < n; i++) {
        os << "|";
        for(unsigned int j = 0; j < m; j++){
            os << matrix[i][j];
            if (j != m-1)
                os << ", ";
        }
        os << "|\n";
    }
    return os;
}
*/
//-----------------------------------------------------------//
//  EXPLICIT INSTANTIATING
//-----------------------------------------------------------//

template class Matrix<char>;
template class Matrix<int>;
template class Matrix<float>;
template class Matrix<double>;

template std::ostream& operator<<(std::ostream& os, const Matrix<char>& p);
template std::ostream& operator<<(std::ostream& os, const Matrix<int>& p);
template std::ostream& operator<<(std::ostream& os, const Matrix<float>& p);
template std::ostream& operator<<(std::ostream& os, const Matrix<double>& p);
