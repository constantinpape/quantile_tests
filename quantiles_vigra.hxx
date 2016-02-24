#pragma once

#include <vector>

#include <vigra/accumulator.hxx>
#include <vigra/multi_array.hxx>

template<class T>
std::vector<T> vigraQuantiles(const vigra::MultiArray<1,T> & values, const size_t NBins = 50)
{
    using namespace vigra;
    using namespace vigra::acc;
    
    typedef StandardQuantiles<AutoRangeHistogram<0> > Quantiles;
    HistogramOptions histogram_opt;
    // apparently, this has to be set at compile time -> ask ulli
    histogram_opt = histogram_opt.setBinCount(NBins);
    
    AccumulatorChain<double, Select<Quantiles> > a;
    a.setHistogramOptions(histogram_opt);
    extractFeatures(values.begin(), values.end(), a);

    TinyVector<T, 7> quants = get<Quantiles>(a);
    std::vector<T> res(5);

    res[0] = quants[1]; 
    res[1] = quants[2]; 
    res[2] = quants[3]; 
    res[3] = quants[4]; 
    res[4] = quants[5]; 

    return res;

}
