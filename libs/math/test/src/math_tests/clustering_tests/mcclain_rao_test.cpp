//
// Created by jakub on 11/14/15.
//

#include <gtest/gtest.h>

#include <math/random.h>
#include <vector>
#include <math/clustering/evaluation/mcclain_rao.h>

/**
 * This source file contains benchmarks for Mcclain Rao cluster evaluation.
 */

//-----------------------------------------------------------//
//  DECLARATIONS
//-----------------------------------------------------------//

/*
 * Creates uniform distributed set of points and
 * computes the clustering index for that data.
 */
void mcclainRaoUniformEvaluation();

/*
 * Generates uniform points, used in:
 * mcclain_rao_uniform_evaluation
 *
 * @count - The number of points.
 * @dim - The dimension of each point
 */
std::vector<const Point<double>*> generateUniformPoints_5Clusters(int count,
                                                                  int dim);
std::vector<const Point<double>*> generateUniformPoints_4Clusters(int count,
                                                                  int dim);
std::vector<const Point<double>*> generateUniformPoints_3Clusters(int count,
                                                                  int dim);
std::vector<const Point<double>*> generateUniformPoints_2Clusters(int count,
                                                                  int dim);

//-----------------------------------------------------------//
//  DEFINITIONS
//-----------------------------------------------------------//

void mcclainRaoUniformEvaluation(){
    int start_k, end_k;
    std::vector<const Point<double>*> data;

    start_k = 2;
    end_k = 10;

    int count = 100;
    int dim = 10;

    //-----------------------------------------------------------//

    data = generateUniformPoints_5Clusters(count, dim);
    std::cout << "//-----------------------------------------------------//\n";
    std::cout << "Clusters: " << 5
        << " Data size: " << data.size() << std::endl;

    McClainRao<double> mc_r5(start_k, end_k);
    mc_r5.compute(&data);

    //-----------------------------------------------------------//

    data = generateUniformPoints_4Clusters(count, dim);
    std::cout << "//-----------------------------------------------------//\n";
    std::cout << "Clusters: " << 4
        << " Data size: " << data.size() << std::endl;

    McClainRao<double> mc_r4(start_k, end_k);
    mc_r4.compute(&data);

    //-----------------------------------------------------------//

    data = generateUniformPoints_3Clusters(count, dim);
    std::cout << "//-----------------------------------------------------//\n";
    std::cout << "Clusters: " << 3
        << " Data size: " << data.size() << std::endl;

    McClainRao<double> mc_r3(start_k, end_k);
    mc_r3.compute(&data);

    //-----------------------------------------------------------//

    data = generateUniformPoints_2Clusters(count, dim);
    std::cout << "//-----------------------------------------------------//\n";
    std::cout << "Clusters: " << 2
        << " Data size: " << data.size() << std::endl;

    McClainRao<double> mc_r2(start_k, end_k);
    mc_r2.compute(&data);

    // Clear memory
    for (std::vector<const Point<double>*>::iterator it = data.begin();
            it!= data.end(); ++it)
    {
        delete (*it);
    }
    data.clear();
}

std::vector<const Point<double>*> generateUniformPoints_5Clusters(int count,
                                                                  int dim){
    const int cluster_count = 5;

    double min1 = 0.0f;
    double max1 = 5.0f;

    double min2 = 10.0f;
    double max2 = 15.0f;

    double min3 = 20.0f;
    double max3 = 25.0f;

    double min4 = 30.0f;
    double max4 = 35.0f;

    double min5 = 40.0f;
    double max5 = 45.0f;

    std::vector<const Point<double>*> data;

    // Cluster 1
    std::vector<const Point<double>*> cluster1 = rnd::uniformPoints
            (
                    count / cluster_count,
                    dim, min1, max1
            );
    data.insert(data.end(), cluster1.begin(), cluster1.end());

    // Cluster 2
    std::vector<const Point<double>*> cluster2 = rnd::uniformPoints
            (
                    count / cluster_count,
                    dim, min2, max2
            );
    data.insert(data.end(), cluster2.begin(), cluster2.end());

    // Cluster 3
    std::vector<const Point<double>*> cluster3 = rnd::uniformPoints
            (
                    count / cluster_count,
                    dim, min3, max3
            );
    data.insert(data.end(), cluster3.begin(), cluster3.end());

    // Cluster 4
    std::vector<const Point<double>*> cluster4 = rnd::uniformPoints
            (
                    count / cluster_count,
                    dim, min4, max4
            );
    data.insert(data.end(), cluster4.begin(), cluster4.end());

    // Cluster 5
    std::vector<const Point<double>*> cluster5 = rnd::uniformPoints
            (
                    count / cluster_count,
                    dim, min5, max5
            );
    data.insert(data.end(), cluster5.begin(), cluster5.end());

    return data;
}

std::vector<const Point<double>*> generateUniformPoints_4Clusters(int count,
                                                                  int dim){
    const int cluster_count = 4;

    double min1 = 0.0f;
    double max1 = 5.0f;

    double min2 = 10.0f;
    double max2 = 15.0f;

    double min3 = 20.0f;
    double max3 = 25.0f;

    double min4 = 30.0f;
    double max4 = 35.0f;


    std::vector<const Point<double>*> data;

    // Cluster 1
    std::vector<const Point<double>*> cluster1 = rnd::uniformPoints
            (
                    count / cluster_count,
                    dim, min1, max1
            );
    data.insert(data.end(), cluster1.begin(), cluster1.end());

    // Cluster 2
    std::vector<const Point<double>*> cluster2 = rnd::uniformPoints
            (
                    count / cluster_count,
                    dim, min2, max2
            );
    data.insert(data.end(), cluster2.begin(), cluster2.end());

    // Cluster 3
    std::vector<const Point<double>*> cluster3 = rnd::uniformPoints
            (
                    count / cluster_count,
                    dim, min3, max3
            );
    data.insert(data.end(), cluster3.begin(), cluster3.end());

    // Cluster 4
    std::vector<const Point<double>*> cluster4 = rnd::uniformPoints
            (
                    count / cluster_count,
                    dim, min4, max4
            );
    data.insert(data.end(), cluster4.begin(), cluster4.end());

    return data;
}

std::vector<const Point<double>*> generateUniformPoints_3Clusters(int count,
                                                                  int dim){
    const int cluster_count = 3;

    double min1 = 0.0f;
    double max1 = 5.0f;

    double min2 = 10.0f;
    double max2 = 15.0f;

    double min3 = 20.0f;
    double max3 = 25.0f;


    std::vector<const Point<double>*> data;

    // Cluster 1
    std::vector<const Point<double>*> cluster1 = rnd::uniformPoints
            (
                    count / cluster_count,
                    dim, min1, max1
            );
    data.insert(data.end(), cluster1.begin(), cluster1.end());

    // Cluster 2
    std::vector<const Point<double>*> cluster2 = rnd::uniformPoints
            (
                    count / cluster_count,
                    dim, min2, max2
            );
    data.insert(data.end(), cluster2.begin(), cluster2.end());

    // Cluster 3
    std::vector<const Point<double>*> cluster3 = rnd::uniformPoints
            (
                    count / cluster_count,
                    dim, min3, max3
            );
    data.insert(data.end(), cluster3.begin(), cluster3.end());

    return data;
}

std::vector<const Point<double>*> generateUniformPoints_2Clusters(int count,
                                                                  int dim){
    const int cluster_count = 2;

    double min1 = 0.0f;
    double max1 = 5.0f;

    double min2 = 10.0f;
    double max2 = 15.0f;


    std::vector<const Point<double>*> data;

    // Cluster 1
    std::vector<const Point<double>*> cluster1 = rnd::uniformPoints
            (
                    count / cluster_count,
                    dim, min1, max1
            );
    data.insert(data.end(), cluster1.begin(), cluster1.end());

    // Cluster 2
    std::vector<const Point<double>*> cluster2 = rnd::uniformPoints
            (
                    count / cluster_count,
                    dim, min2, max2
            );
    data.insert(data.end(), cluster2.begin(), cluster2.end());

    return data;
}

//-----------------------------------------------------------//
//  TESTS
//-----------------------------------------------------------//

/*
 * Testing distance
 */
TEST(McClain_Rao, Uniform_Generated_Points) {
    //mcclainRaoUniformEvaluation();
}
