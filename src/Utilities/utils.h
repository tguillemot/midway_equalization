#ifndef _UTILITIES_UTILS_H
#define _UTILITIES_UTILS_H

#include "mt19937ar.h"

inline float GenerateGaussianNoise(
        const float& sigma
        )
{
    if(!sigma) return 0.f;
    const double a = mt_genrand_res53();
    const double b = mt_genrand_res53();
    return sigma * (float) (sqrtl(-2.0l * log(a)) * cos(2.0l * M_PI * b));
}

inline float Crop(
        const float& value,
        const float& minimum,
        const float& maximum
        )
{
    return (value > maximum) ? maximum : (value < minimum) ? minimum : value;
}

#endif //_UTILITIES_UTILS_H
