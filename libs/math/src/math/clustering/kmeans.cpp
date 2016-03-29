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
#include <random>
#include <stdexcept>
#include "math/clustering/kmeans.h"
#include "math/geometry.h"   /* distance */
#include <time.h>       /* time */

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

template <class T>
KMeans<T>::KMeans(int k, double tol, int max_iter) :
        k(k), tol(tol), max_iter(max_iter) {
}

template <class T>
KMeans<T>::KMeans(const KMeans& cpy){
    this->k = cpy.k;
    this->max_iter = cpy.max_iter;
    this->tol = cpy.tol;
    this->dataDimension = cpy.dataDimension;

    this->clusterIndices = cpy.clusterIndices;
    this->centroids = cpy.centroids;
    this->previousCentroids = cpy.previousCentroids;

    this->data = data;

    this->clusters = cpy.clusters;
}

template <class T>
KMeans<T>::~KMeans() {
    data = NULL;
}


//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

/*
 * Starts the computations for kmeans.
 *
 * 1) Initiate cluster with kmeans_pp algorithm
 *  1.1) Update the clusters, assigned each point to new clusters
 * 2) Update centroids.
 * 3) Update clusters
 * 4) Repeat 2), 3) until convergence or max iterations.
 */
template <class T>
void KMeans<T>::compute(const std::vector<const Point<T>*>* data) {
    int t;

    if((*data).size() == 0){
        return;
    }

    this->data = data;
    this->dataDimension = (*(*data)[0]).size();

    // Start with initializing centroids and assigning points to them
    kmeans_pp();
    updateClusters();

    t = 0;
    while(!isConverged(t++)) {
        updateCentroids();

        updateClusters();
    }

    // Create clusters dynamically.
    this->createClusters();

}

//-----------------------------------------------------------//
//  GETTERS / SETTERS
//-----------------------------------------------------------//

/*
 * Returns i-th cluster.
 * The valid range of i = [0, k-1]
 */
template <class T>
std::vector<const Point<T>*> KMeans<T>::getCluster(int i){
    if( i < 0 || i >= k)
        throw std::invalid_argument("Index out of range");

    return this->clusters[i];
}

template <class T>

const std::vector<int>* KMeans<T>::getClusterIndices(int i) const{
    if( i < 0 || i >= k)
        throw std::invalid_argument("Index out of range");
    return &(this->clusterIndices[i]);
}


template <class T>
int KMeans<T>::getK() {
    return this->k;
}

template <class T>
int KMeans<T>::getMaxIter() {
    return this->max_iter;
}

template <class T>
double KMeans<T>::getTol() {
    return this->tol;
}

//-----------------------------------------------------------//

template <class T>
void KMeans<T>::setK(int k) {
    this->k = k;
}

template <class T>
void KMeans<T>::setMaxIter(int max_iter) {
    this->max_iter = max_iter;
}

template <class T>
void KMeans<T>::setTol(double tol) {
    this->tol = tol;
}

//-----------------------------------------------------------//
//  PRIVATE METHODS
//-----------------------------------------------------------//

// -----------------------------------------------------------------------------

/*
 * Computes initial values for centroids.
 * The kmeans++ algorithm is used.
 *
 * 1) Choose one center uniformly at random from among the data point.
 *
 * 2) For each data point x, compute D(x), the distance between x and
 *      the nearest center that has already been chosen.
 *
 * 3) Choose one new data point at random as new center, using a weighted
 *      probability distribution where a point x is chosen with
 *      probability proportional to D(x)^2
 *
 * 4) Repeat steps 2) and 3) until k centers have chosen.
 *
 */
template <class T>
void KMeans<T>::kmeans_pp(){
    Point<T> center;
    int centroidCount;
    int dataSize;
    int centerIndex;

    srand(time(NULL));

    // Current centroid count, must reach value k
    centroidCount = 0;
    dataSize = (*data).size();
    // Clear centroids
    this->centroids.clear();
    this->centroids.resize(k);

    /* 1) Choose first center. */
    // Generates random number between [0, dataSize-1]

    centerIndex = rand() % (dataSize);
    center = *((*data)[centerIndex]);
    this->centroids[centroidCount++] = center;

    while(centroidCount < k){
        /* 2) Compute D(x) */
        this->chooseCentroid(centroidCount);
    }
}

/*
 * Chooses next centroid for kmeans++ algorithm.
 *
 * Computes the distance D(x) of each point x to its nearest centroid.
 * Then chooses next centroid among data points based on discrete
 * distribution with distribution proportional to D(x).
 */
template <class T>
void KMeans<T>::chooseCentroid(int &centroidCount){
    int dataSize = (*data).size();
    std::vector<double> squaredDist(dataSize);

    // For each point find the distance to closest center.
    for(int i = 0;i < dataSize; i++){
        double minDistance;
        int minIndex;

        minIndex = 0;
        minDistance = math::euclideanDistance(this->centroids[minIndex],
                                             *((*data)[i]));

        for(int j = 0;j < centroidCount; j++){
            double distance = math::euclideanDistance(this->centroids[j],
                                                     *((*data)[i]));
            if(minDistance >= distance ){
                minDistance = distance;
                minIndex = j;
            }
        }
        squaredDist[i] = (minDistance*minDistance);
    }

    /* 3) Weighted Distribution */

    std::default_random_engine gen;
    std::discrete_distribution<> dist(squaredDist.begin(), squaredDist.end());
    int centerIndex = dist(gen);

    this->centroids[centroidCount++] = *((*data)[centerIndex]);
}

// -----------------------------------------------------------------------------


/*
 * Updating centroids.
 * Save the previous centroids.
 * Compute new ones based on the center of gravity of the cluster.
 */
template <class T>
void KMeans<T>::updateCentroids(){
    int clusterSize;

    // Save the previous centroids
    this->previousCentroids = this->centroids;

    // Compute the center of gravity of each cluster.
    for(int i = 0;i < this->k; i++){
        Point<T> centroid(this->dataDimension);
        clusterSize = this->clusterIndices[i].size();

        for(int j = 0; j < clusterSize; j++){
            int l = this->clusterIndices[i][j];
            centroid += *((*data)[l]);
        }

        centroid /= clusterSize;
        this->centroids[i] = centroid;
    }

}

// -----------------------------------------------------------------------------

/*
 * Cluster is updated. Each point in data finds the closest centroid.
 * Point which is assigned k-th centroid, belongs to k-th cluster
 */
template <class T>
void KMeans<T>::updateClusters(){
    int minClusterIndex;
    double distance, minClusterDistance;
    int dataSize;

    dataSize = (*data).size();

    // Clear the indices
    this->clusterIndices.clear();
    this->clusterIndices.resize(k);

    // For each point in data, find its closest centroid
    for(int i = 0;i < dataSize;i++){
        Point<T> point = *((*data)[i]);

        minClusterIndex = 0;
        minClusterDistance =
                math::euclideanDistance(point,
                                        this->centroids[minClusterIndex]);

        // Find closest centroid
        for(int j = 0;j < k; j++){
            distance = math::euclideanDistance(point,
                                               this->centroids[j]);

            if(minClusterDistance >= distance){
                minClusterDistance = distance;
                minClusterIndex = j;
            }
        }
        this->clusterIndices[minClusterIndex].push_back(i);
    }

}

// -----------------------------------------------------------------------------

/*
 * KMeans is converged when all centroids have not repositioned by more than
 * tol value since last iteration
 */
template <class T>
bool KMeans<T>::isConverged(const int& t){
    int flags;
    double distance;

    if (t == 0) {
        return false;
    }
    if (t > max_iter) {
        return true;
    }

    // When flags reaches value k, then all centroids have converged.
    flags = 0;
    for(int i = 0;i < this->k; i++){
        distance = math::euclideanDistance(this->centroids[i],
                                          this->previousCentroids[i]);

        if(distance <= this->tol)
            flags++;
    }

    if (flags == k){
        return true;
    }
    else{
        return false;
    }
}

// -----------------------------------------------------------------------------

/*
 * Creates clusters based on the indices.
 */
template <class T>
void KMeans<T>::createClusters(){
    this->clusters.clear();

    for(int i = 0; i < this->k ;i++){
        std::vector<const Point<T>*> cluster;

        unsigned int size = clusterIndices[i].size();

        for(unsigned int j = 0; j < size; j++){
            int index = this->clusterIndices[i][j];

            const Point<T>* p = (*data)[index];
            cluster.push_back(p);
        }
        this->clusters.push_back(cluster);
    }
}

// -----------------------------------------------------------------------------

//-----------------------------------------------------------//
//  EXPLICIT TEMPLATE  INSTANTIATING
//-----------------------------------------------------------//

template class KMeans<char>;
template class KMeans<int>;
template class KMeans<float>;
template class KMeans<double>;