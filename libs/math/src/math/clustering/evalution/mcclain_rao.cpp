//
// Created by jakub on 11/14/15.
//

#include "math/clustering/evaluation/mcclain_rao.h"
#include "math/geometry.h"
#include <cstring>
#include <stdexcept>

template <class T>
McClainRao<T>::McClainRao(int start_k, int end_k)
        : start_k(start_k), end_k(end_k) {
}

template <class T>
McClainRao<T>::~McClainRao(){
    delete(best_km);
}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

/*
 * Starts the computation for mcclain-rao index
 */
template <class T>
void McClainRao<T>::compute(const std::vector<const Point<T>*>* data) {
    double C;
    this->data = data;
    this->best_C = -1;

    if(this->data == NULL){
        throw std::invalid_argument("NULL Pointer");
    }

    // Iterate through a fixed interval of possible values of k
    for (int k = start_k; k <= end_k; k++){
        double tol = 0.001;
        int max_iter = 1000;
        km = new KMeans<T>(k, tol, max_iter);

        // Compute clustering
        km->compute(data);

        // Compute the sum within
        computeSumWithin();

        // Compute the sum between
        computeSumBetween();

        // Compute the index.
        C = computeIndex();

        //std::cout << "mc-r(" << k << ") = " << C << std::endl;

        if(best_C > C || best_C == -1){
            best_C = C;
            best_km = new KMeans<T>(*km);
        }

        delete km;
    }
}

template <class T>
KMeans<T>* McClainRao<T>::getBestClustering(){
    return this->best_km;
}


//-----------------------------------------------------------//
//  PRIVATE METHODS
//-----------------------------------------------------------//

template <class T>
void McClainRao<T>::computeSumWithin() {
    this->sumWithin = 0;
    this->nw = 0;

    // For each cluster
    for(int c = 0; c < km->getK(); c++){
        std::vector<const Point<T>*> cluster = km->getCluster(c);
        int size = cluster.size();

        // If some cluster is empty. return with bad result
        if(size == 0) {
            this->nw = 0;
            return;
        }

        for(int i = 0; i < size-1; i++){
            for(int j = i+1; j < size; j++){
                this->sumWithin += math::euclideanDistance(*cluster[i],
                                                          *cluster[j]);
                this->nw++;
            }
        }
    }

}

template <class T>
void McClainRao<T>::computeSumBetween() {
    this->sumBetween = 0;
    this->nb = 0;

    // Number of clusters
    int k = km->getK();

    // For each distinct pair of clusters
    for(int c1 = 0; c1 < k-1; c1++) {
        std::vector<const Point<T>*> cluster1 = km->getCluster(c1);

        for(int c2 = c1+1; c2 < k; c2++){
            std::vector<const Point<T>*> cluster2 = km->getCluster(c2);

            // If some cluster is empty. return with bad result
            if(cluster1.size() == 0 || cluster2.size() == 0){
                this->nb = 0;
                return;
            }

            // For each pair of points from cluster1 and cluster2
            for(unsigned int i = 0;i < cluster1.size(); i++){
                for(unsigned int j = 0; j < cluster2.size(); j++){
                    this->sumBetween += math::euclideanDistance(*cluster1[i],
                                                               *cluster2[j]);
                    this->nb++;
                }
            }
        }
    }
}

template <class T>
double McClainRao<T>::computeIndex(){
    double C;

    // If at least one cluster was empty, set the result to worst one.
    if(this->nw == 0 || this->nb == 0)
        C = 1.0f;
    else
        C = (this->sumWithin/this->nw) / (this->sumBetween/this->nb);

    return C;
}

//-----------------------------------------------------------//
//  EXPLICIT TEMPLATE INSTANTIATING
//-----------------------------------------------------------//

template class McClainRao<char>;
template class McClainRao<int>;
template class McClainRao<float>;
template class McClainRao<double>;