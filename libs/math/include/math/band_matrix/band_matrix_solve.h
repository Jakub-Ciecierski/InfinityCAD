//
// Created by jakub on 4/20/16.
//

#ifndef IC_BAND_MATRIX_SOLVE_H
#define IC_BAND_MATRIX_SOLVE_H

/*
 * The source code and idea comes from:
 * http://www.aip.de/groups/soe/local/numres/bookcpdf/c2-4.pdf
 */

#include <math.h>
#include <iostream>

#define SWAP(a,b) {dum=(a);(a)=(b);(b)=dum;}
#define TINY 1.0e-20

inline int MAX(int i, int j) { return (i>j ? i : j);}
inline int MIN(int i, int j) { return (i<j ? i : j);}


void printVec(float* a, int n){
    for(int i = 1; i < n+1; i++){
        std::cout << a[i] << ", ";
    }
    std::cout << std::endl << std::endl;
}

void printMat(float** a, int n, int m){
    for(int i = 1; i < n+1; i++){
        for(int j = 1; j < m+1; j++){
            std::cout << a[i][j] << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
}

/*
Matrix multiply b = A · x, where A is band diagonal with m1 rows below the diagonal and m2
rows above. The input vector x and output vector b are stored as x[1..n] and b[1..n],
respectively. The array a[1..n][1..m1+m2+1] stores A as follows: The diagonal elements
are in a[1..n][m1+1]. Subdiagonal elements are in a[j..n][1..m1] (with j > 1 appropriate
to the number of elements on each subdiagonal). Superdiagonal elements are in
a[1..j][m1+2..m1+m2+1] with j < n appropriate to the number of elements on each superdiagonal.
 */
void banmul(float **a, unsigned long n, int m1, int m2, float x[], float b[])
{
    unsigned long i,j,k,tmploop;
    for (i=1;i<=n;i++) {
        k=i-m1-1;
        tmploop=MIN(m1+m2+1,n-k);
        b[i]=0.0;
        for (j=MAX(1,1-k);j<=tmploop;j++)
            b[i] += a[i][j]*x[j+k];
    }
}

/*
Given an n × n band diagonal matrix A with m1 subdiagonal rows and m2
superdiagonal rows, compactly stored in the array a[1..n][1..m1+m2+1] as
described in the comment for routine
banmul, this routine constructs an LU decomposition of a rowwise permutation of A. The upper
triangular matrix replaces a, while the lower triangular matrix is returned in al[1..n][1..m1].
indx[1..n] is an output vector which records the row permutation effected by the partial
pivoting; d is output as ±1 depending on whether the number of row interchanges was even
or odd, respectively. This routine is used in combination with banbks to solve band-diagonal
sets of equations.
 */
void bandec(float **a, unsigned long n, int m1, int m2, float **al,
            unsigned long indx[], float *d) {
    unsigned long i,j,k,l;
    int mm;
    float dum;
    mm=m1+m2+1;
    l=m1;
    for (i=1;i<=m1;i++) {
        for (j=m1+2-i;j<=mm;j++)
            a[i][j-l]=a[i][j];
        l--;
        for (j=mm-l;j<=mm;j++)
            a[i][j]=0.0;
    }
    *d=1.0;
    l=m1;
    for (k=1;k<=n;k++) {
        dum=a[k][1];
        i=k;
        if (l < n) l++;
        for (j=k+1;j<=l;j++) {
            if (fabs(a[j][1]) > fabs(dum)) {
            dum=a[j][1];
            i=j;
            }
        }
        indx[k]=i;
        if (dum == 0.0)
            a[k][1]=TINY;

        if (i != k) {
            *d = -(*d);
            for (j=1;j<=mm;j++)
                SWAP(a[k][j],a[i][j]);
        }
        for (i=k+1;i<=l;i++) {
            dum=a[i][1]/a[k][1];
            al[k][i-k]=dum;
            for (j=2;j<=mm;j++)
                a[i][j-1]=a[i][j]-dum*a[k][j];
            a[i][mm]=0.0;
        }
    }
}

/*
Given the arrays a, al, and indx as returned from bandec, and given a right-hand side vector
b[1..n], solves the band diagonal linear equations A · x = b. The solution vector x overwrites
b[1..n]. The other input arrays are not modified, and can be left in place for successive calls
with different right-hand sides.
*/
void banbks(float **a, unsigned long n, int m1, int m2, float **al,
            unsigned long indx[], float b[])
{
    unsigned long i,k,l;
    int mm;
    float dum;
    mm=m1+m2+1;
    l=m1;
    for (k=1;k<=n;k++) {
        i=indx[k];
        if (i != k)
            SWAP(b[k],b[i]);
        if (l < n)
            l++;
        for (i=k+1;i<=l;i++)
            b[i] -= al[k][i-k]*b[k];
    }
    l=1;
    for (i=n;i>=1;i--) {
        dum=b[i];
        for (k=2;k<=l;k++)
            dum -= a[i][k]*b[k+i-1];
        b[i]=dum/a[i][1];
        if (l < mm)
            l++;
    }
}

#endif //IC_BAND_MATRIX_SOLVE_H
