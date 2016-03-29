//
// Created by jakub on 11/10/15.
//

#ifndef AC_POINT_H
#define AC_POINT_H

#include <iostream>
#include <vector>
#include <string>
#include <math.h>

/**
 * Class template for Point
 *
 */
template <class T>
class Point {
private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    // Stores the elements of the point
    std::vector<T> elements;

public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    /*
     * Creates empty point. No dimensions
     */
    Point();

    /*
     * Creates point with size dimensions
     *
     * @size - The amount of dimensions
     */
    Point(int size);

    /*
     * Creates a point from data.
     *
     * @data - The data from which point is created.
     * @count - Number of elements in @data.
     */
    Point(T* data, int count);

    Point(const Point& p);

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    /*
     * Adds another dimension to the point.
     */
    void addDimension();


    /*
     * Adds another count dimensions to the point.
     */
    void addDimensions(int count);


    /*
     * Adds another dimension with element value
     *
     * @element - The element to assign to the new dimension
     */
    void addDimension(T element);

    /*
     * Remove dimension
     */
    void removeDimension();

    /*
     * Returns the size (number of dimensions)
     */
    int size() const;

    /*
     * Returns string with point's representations
     */
    std::string toString();

    //-----------------------------------------------------------//
    //  OPERATORS
    //-----------------------------------------------------------//

    /*
     * Returns ith element.
     */
    T &operator[](int i);
    const T &operator[](int i) const;

    /*
     * Adds two points
     */
    Point<T> operator+(const Point<T>& p) const;
    void operator+=(const Point<T>& p);

    /*
     * Adds value t to each dimension.
     * Watch out for implicit casting.
     */
    Point<T> operator+(const T& t) const;
    void operator+=(const T& p);

    /*
     * Subtracts two points
     */
    Point<T> operator-(const Point<T>& p) const;

    /*
     * Divides by divider.
     * Watch out for implicit casting.
     */
    Point<T> operator/(const T& divider);
    void operator/=(const T& divider);

    /*
     * Two points are equal if all corresponding elements
     * are equal
     */
    bool operator==(const Point<T>& p) const;

    /*
     * Two points are not equal if at least one pair
     * of corresponding elements is not equal
     */
    bool operator!=(const Point<T>& p) const;

    template <typename TT>
    friend std::ostream& operator<<(std::ostream& os, const Point<TT>& p);

    /* Multiply every entry by a scalar */
    Point<T> operator* (T x) const;
};

#endif //AC_POINT_H
