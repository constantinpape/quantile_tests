#pragma once

#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <iostream>

#include <vigra/multi_array.hxx>

template<class T>
std::vector<T> exactQuantiles(const vigra::MultiArray<1,T> & values)
{
    size_t size = static_cast<size_t>( values.shape(0) );
    double eps = 1.e-5;
    
    // we copy to a vector to sort
    std::vector<T> values_sorted(size);
    std::copy( values.begin(), values.end(), values_sorted.begin() );

    // I hope sort likes vigra multi arrays...
    std::sort( values_sorted.begin(), values_sorted.end() );

    // initialize the correct quantilte positions
    std::vector<double> quantiles{ { 0.1, 0.25, 0.5, 0.75, 0.9 } };     
    std::vector<T> res( quantiles.size() );
    for( size_t i = 0; i < quantiles.size(); i++ )
    {
        // declare the resulting quantile
        T quant_res;
        // exact quantile position
        double quant = quantiles[i];
        // here we chose n - 1, because we regard n datapoints,
        // seperated by n - 1 delimiters.
        // cf. np.percentile(interpolation = 'linear')
        double pos   = quant * (size - 1);
        // check whether the position is integral or decimal
        if( pos == std::floor(pos)) // if true integral and we are at exact quantile
        {
            size_t q_index = static_cast<size_t>( std::floor(pos) );
            quant_res = values_sorted.at(q_index);
        }
        else // if false decimal and we have to interpolate
        {
            // calculate the interpolated quantile
            size_t lower_index = static_cast<size_t>( std::floor(pos) );
            size_t upper_index = lower_index + 1;
            double part = pos - lower_index;
            quant_res = values_sorted.at(lower_index);
            quant_res += part * ( values_sorted.at(upper_index) -  values_sorted.at(lower_index) ); 
        }
        res[i] = quant_res;
    }
    
    return res;
}
