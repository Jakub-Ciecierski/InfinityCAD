//
// Created by jakub on 12/3/15.
//

#ifndef AC_MATRIX_H
#define AC_MATRIX_H

#include <iostream>
#include <vector>
#include <stdexcept>

/*
 * Matrix n by m.
 * n - rows.
 * m - columns.
 */
template <class T>
class Matrix {
private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    std::vector<std::vector<T>> entries;

    unsigned int n;
    unsigned int m;

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

public:
    Matrix();
    Matrix(unsigned int n, unsigned int m);
    Matrix(unsigned int n, unsigned int m, T val);

    Matrix(std::vector<std::vector<T>> entries,
           unsigned int n, unsigned int m);

    Matrix(const Matrix& matrix);

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    void resize(unsigned int n, unsigned int m);

    std::vector<T> getColumn(unsigned int i) const;
    void setColumn(unsigned int i, const std::vector<T>& vec);

    /*
     * Sets entry of i-th row, j-th column
     */
    void setEntry(unsigned int i, unsigned int j, const T& value);

    /*
     * Gets entry of i-th row, j-th column
     */
    const T& getEntry(unsigned int i, unsigned int j) const;

    unsigned int rowCount() const;
    unsigned int columnCount() const;

    //-----------------------------------------------------------//
    //  OPERATORS
    //-----------------------------------------------------------//

    /*
     * Returns i-th row
     */
    std::vector<T>& operator[](unsigned int i);
    const std::vector<T>& operator[](unsigned int i) const;

    template <typename TT>
    friend std::ostream& operator<<(std::ostream& os, const Matrix<TT>& matrix);
};

template <class T>
Matrix<T>::Matrix(){
    n = 0;
    m = 0;
}

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
Matrix<T>::Matrix(unsigned int n, unsigned int m, T val) : n(n), m(m){
    entries.resize(n);

    for(unsigned int i = 0; i < n; i++){
        entries[i].resize(m);
    }

    for(unsigned int i = 0; i < n; i++){
        for(unsigned int j = 0; j < m; j++){
            entries[i][j] = val;
        }
    }
}

template <class T>
Matrix<T>::Matrix(std::vector<std::vector<T>> entries,
                  unsigned int n, unsigned int m) :
        entries(entries), n(n), m(m){
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
void Matrix<T>::resize(unsigned int n, unsigned int m) {
    int currentN = this->n;
    int currentM = this->m;

    this->n = n;
    this->m = m;
    std::vector<std::vector<T>> newEntries;

    newEntries.resize(n);

    for(unsigned int i = 0; i < n; i++){
        newEntries[i].resize(m);
    }
    for(unsigned int i = 0; i < n; i++){
        for(unsigned int j = 0; j < m; j++){
            if(i < currentN && j < currentM)
                newEntries[i][j] = entries[i][j];
        }
    }
    entries = newEntries;
}


template <class T>
std::vector<T> Matrix<T>::getColumn(unsigned int i) const{
    if(i < 0 || i >= m)
        throw std::invalid_argument("Array out of Bound. Matrix::setEntry");

    std::vector<T> column(n);
    for(unsigned int j = 0; j < n; j++){
        column[j] = entries[j][i];
    }
    return column;
}

template <class T>
void Matrix<T>::setColumn(unsigned int i, const std::vector<T>& vec){
    if(i < 0 || i >= m)
        throw std::invalid_argument("Array out of Bound. Matrix::setEntry");

    for(unsigned int j = 0; j < n; j++){
        entries[j][i] = vec[j];
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



#endif //AC_MATRIX_H
