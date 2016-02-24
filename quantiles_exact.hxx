#include <vector>
#include <algorithm>
#include <iostream>

#include <vigra/multi_array.hxx>

template<class T>
std::vector<T> exactQuantiles(const vigra::MultiArray<1,T> & values)
{
    size_t size = static_cast<size_t>( values.shape(0) );
    
    // we copy to a vector to sort
    std::vector<T> values_sorted(size);
    std::copy( values.begin(), values.end(), values_sorted.begin() );

    // I hope sort likes vigra multi arrays...
    std::sort( values_sorted.begin(), values_sorted.end() );

    // TODO are all these ternarys correct?
    // get 0.1 quantile
    size_t index1 = size / 10;
    T q1  = (size % 2 == 0) ? (values_sorted[index1] + values_sorted[index1+1])/2. : values_sorted[index1]; 
    
    // get 0.25 quantile
    size_t index25 = size / 4;
    T q25  = (size % 2 == 0) ? (values_sorted[index25] + values_sorted[index25+1])/2. : values_sorted[index25]; 
    
    // get the 0.5 quantile / median
    size_t index5 = size / 2;
    T q5  = (size % 2 == 0) ? (values_sorted[index5] + values_sorted[index5+1])/2. : values_sorted[index5]; 
    
    // get the 0.75 quantile
    size_t index75 = 3 * size / 4;
    T q75  = (size % 2 == 0) ? (values_sorted[index75] + values_sorted[index75+1])/2. : values_sorted[index75]; 

    // get the 0.9 quantile
    size_t index9 = 9 * size / 10;
    T q9  = (size % 2 == 0) ? (values_sorted[index9] + values_sorted[index9+1])/2. : values_sorted[index9]; 

    std::vector<T> res(5);

    res[0] = q1;
    res[1] = q25;
    res[2] = q5;
    res[3] = q75;
    res[4] = q9;

    return res;
}
