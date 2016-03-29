//
// Created by jakub on 12/3/15.
//

#ifndef AC_MATRIX_H
#define AC_MATRIX_H

#include <iostream>
#include <vector>

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
    Matrix(unsigned int n, unsigned int m);

    Matrix(const Matrix& matrix);

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

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


#endif //AC_MATRIX_H
