//
// Created by jakub on 11/12/15.
//

#include "math/random.h"

namespace rnd
{
    template <typename T>
    std::vector<const Point<T>*> uniformPoints(int count,
                                               int dim, T min, T max){
        std::vector<const Point<T>*> points(count);

        std::random_device                  rand_dev;
        std::mt19937                        gen(rand_dev());
        std::uniform_real_distribution<>    dist(min, max);

        // Generate count points
        for(int i = 0;i < count; i++){
            Point<T>* p = new Point<T>(dim);

            // Generate each dimension.
            for(int j = 0; j < dim; j++){
                (*p)[j] = dist(gen);
            }
            points[i] = p;
        }
        return points;
    }

    //-----------------------------------------------------------//
    //  TEMPLATE DECLARATIONS
    //-----------------------------------------------------------//

    template std::vector<const Point<char>*> uniformPoints(int count,
                                                      int dim,
                                                      char min, char max);

    template std::vector<const Point<int>*> uniformPoints(int count,
                                                    int dim,
                                                    int min, int max);

    template std::vector<const Point<float>*> uniformPoints(int count,
                                                      int dim,
                                                      float min, float max);

    template std::vector<const Point<double>*> uniformPoints(int count,
                                                      int dim,
                                                      double min, double max);
}
