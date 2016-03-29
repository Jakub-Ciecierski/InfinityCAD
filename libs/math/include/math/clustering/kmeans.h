//
// Created by jakub on 11/10/15.
//

#ifndef AC_KMEANS_H
#define AC_KMEANS_H

#include <vector>
#include "math/point.h"

/*
 * Computes clustering Kmeans algorithm.
 *
 * 1) Randomly select k center
 * 2) Group points to their closest centroids
 * 3) update centroids by computing center of gravity - mean
 * 4) Repeat 2), 3) until convergence or after maximum iterations.
 *
 */
template <class T>
class KMeans {

private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    // Number of clusters
    int k;

    // The distance limit before converging
    double tol;

    // Maximum number of iteration
    int max_iter;

    // Number of dimension of the data;
    int dataDimension;

    // Each vector is Cluster having a vector of indices
    // Each index i corresponds to the i-th element in the data set.
    std::vector<std::vector<int>> clusterIndices;

    // Centroid of each cluster
    std::vector<Point<T>> centroids;

    // The centroids of previous iteration.
    std::vector<Point<T>> previousCentroids;

    // Point to the data to be cluster
    const std::vector<const Point<T>*>* data;

    // Vector of clusters, each containing points
    std::vector<std::vector<const Point<T>*>> clusters;

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

    /*
     * Initializes centroids with kmeans++ algorithm
     */
    void kmeans_pp();

    /*
     * Choose next centroid, used in kmeans_pp.
     *
     * @centroidCount - Current number of centroids
     */
    void chooseCentroid(int &centroidCount);

    /*
     * Update centroids position
     */
    void updateCentroids();

    /*
     * Updates clusters based on current centroids position.
     */
    void updateClusters();

    /*
     * Checks whether the computation converged.
     *
     * @t - the current iteration number.
     */
    bool isConverged(const int& t);

    void createClusters();

public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    /*
     * @k - the number of clusters
     * @tol - the tolerance of convergence.
     * @max_iter - maximum number of iterations.
     */
    KMeans(int k, double tol, int max_iter);

    KMeans(const KMeans& cpy);

    ~KMeans();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    /*
     * Starts the computations.
     *
     * @data - the data to compute the kmeans for
     */
    void compute(const std::vector<const Point<T>*>* data);

    //-----------------------------------------------------------//
    //  GETTERS / SETTERS
    //-----------------------------------------------------------//

    /*
     * Returns i-th cluster.
     * The valid range of i = [0, k-1].
     *
     * @i - the index of cluster
     */
    std::vector<const Point<T>*> getCluster(int i);

    /*
     * Returns i-th cluster indices
     *
     * @i - the index of cluster.
     */
    const std::vector<int>* getClusterIndices(int i) const;

    int getK();
    double getTol();
    int getMaxIter();

    void setK(int k);
    void setTol(double tol);
    void setMaxIter(int max_iter);
};


#endif //AC_KMEANS_H
