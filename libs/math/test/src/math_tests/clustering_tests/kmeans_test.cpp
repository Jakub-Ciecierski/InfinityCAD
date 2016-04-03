//
// Created by jakub on 11/10/15.
//

#include <gtest/gtest.h>
#include <vector>

#include <math/clustering/kmeans.h>
#include <iostream>

//-----------------------------------------------------------//
//  DECLARATIONS
//-----------------------------------------------------------//

/*
 * Creates small set of data with 2 clusters and computes clustering
 */
void kmeansSmall2Clusters();

/*
 * Creates an KMeans object
 */
KMeans<double> createKMeans();

//-----------------------------------------------------------//
//  DEFINITIONS
//-----------------------------------------------------------//

void kmeansSmall2Clusters(){
    Point<double>* p1 = new Point<double>(2);
    (*p1)[0] = 3.0f;
    (*p1)[1] = 5.0f;

    Point<double>* p2 = new Point<double>(2);
    (*p2)[0] = 5.0f;
    (*p2)[1] = 3.0f;

    Point<double>* p3 = new Point<double>(2);
    (*p3)[0] = 4.0f;
    (*p3)[1] = 4.0f;


    Point<double>* p4 = new Point<double>(2);
    (*p4)[0] = 10.0f;
    (*p4)[1] = 9.0f;

    Point<double>* p5 = new Point<double>(2);
    (*p5)[0] = 9.0f;
    (*p5)[1] = 10.0f;

    Point<double>* p6 = new Point<double>(2);
    (*p6)[0] = 8.0f;
    (*p6)[1] = 8.0f;

    std::vector<const Point<double>*> data = {p1, p2, p3, p4 ,p5 ,p6};

    KMeans<double> km = createKMeans();

    km.compute(&data);

    for(int c = 0; c < km.getK(); c++){
        std::cout << "Cluster #" << c << std::endl;

        std::vector<const Point<double>*> cluster = km.getCluster(c);
        for(unsigned int i = 0; i < cluster.size(); i++){
            std::cout << (*cluster[i]) << std::endl;
        }
    }

    std::cout << "Clearing memory" << std::endl;

    for(unsigned int i = 0; i < data.size(); i++){
        delete data[i];
    }
    data.clear();
}

KMeans<double> createKMeans(){
    int k, max_iter;
    double tol;

    k = 2;
    max_iter = 100;
    tol = 0.001;

    KMeans<double> km(k, tol, max_iter);

    return km;
}

//-----------------------------------------------------------//
//  TESTS
//-----------------------------------------------------------//

TEST(KMeans, GetK_GetMaxIter_GetTol) {
    int expected_k, actual_k;
    int expected_max_iter, actual_max_iter;
    double expected_tol, actual_tol;

    actual_k = 4;
    actual_max_iter = 20;
    actual_tol = 0.001;

    KMeans<int> km(actual_k, actual_tol, actual_max_iter);

    expected_k = km.getK();
    expected_max_iter = km.getMaxIter();
    expected_tol = km.getTol();

    EXPECT_EQ(expected_k, actual_k);
    EXPECT_EQ(expected_max_iter, actual_max_iter);
    EXPECT_EQ(expected_tol, actual_tol);
}


TEST(KMeans, GetCluster_OutOfRange_InvalidArgument) {
    KMeans<double> km = createKMeans();

    EXPECT_THROW(km.getCluster(-1), std::invalid_argument);
    EXPECT_THROW(km.getCluster(km.getK()), std::invalid_argument);
}

TEST(KMeans, Compute) {
    //kmeansSmall2Clusters();
}