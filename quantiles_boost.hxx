#pragma once

#include <cmath>
#include <vector>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/tail_quantile.hpp>
#include <boost/accumulators/statistics.hpp>

#include <vigra/multi_array.hxx>

namespace bacc = boost::accumulators;

// hide this
namespace
{

template<class T>
T replaceRotten(const T & val, const T & replaceVal)
 {
     if( std::isfinite(val) )
         return val;
     else
         return replaceVal;
 }


template<class T>
class QuantilesRightTailed
{
public:

    typedef std::integral_constant<int, 5> NFeatures;

    typedef bacc::accumulator_set<
        T, 
        bacc::stats<bacc::tag::mean, bacc::tag::tail_quantile<bacc::right> > 
    > AccumulatorType;

    QuantilesRightTailed(const size_t rightTailCacheSize = 1000)
    :   acc_(bacc::right_tail_cache_size = rightTailCacheSize)
    {

    }
    
    QuantilesRightTailed & acc(const T & val)
    {
        acc_(val);
        return *this;
    }

    template<class RESULT_ITER>
    void result(RESULT_ITER rBegin, RESULT_ITER rEnd)
    {
        using namespace boost::accumulators;
        auto mean = extract_result< tag::mean >(acc_);
        rBegin[0]  = replaceRotten(quantile(acc_, quantile_probability = 0.1 ),mean);  
        rBegin[1]  = replaceRotten(quantile(acc_, quantile_probability = 0.25 ),mean); 
        rBegin[2]  = replaceRotten(quantile(acc_, quantile_probability = 0.5 ),mean);  
        rBegin[3] = replaceRotten(quantile(acc_, quantile_probability = 0.75 ),mean); 
        rBegin[4] = replaceRotten(quantile(acc_, quantile_probability = 0.90 ),mean); 
    }

private:

    T replaceRotten(const T & val, const T & replaceVal)
    {
        if( std::isfinite(val) )
            return val;
        else
            return replaceVal;
    }

    AccumulatorType acc_;

};


template<class T>
class QuantilesLeftTailed
{
public:

    typedef std::integral_constant<int, 5> NFeatures;

    typedef bacc::accumulator_set<
        T, 
        bacc::stats<bacc::tag::mean, bacc::tag::tail_quantile<bacc::left> > 
    > AccumulatorType;

    QuantilesLeftTailed(const size_t leftTailCacheSize = 1000)
    :   acc_(bacc::left_tail_cache_size = leftTailCacheSize)
    {

    }
    
    QuantilesLeftTailed & acc(const T & val)
    {
        acc_(val);
        return *this;
    }

    template<class RESULT_ITER>
    void result(RESULT_ITER rBegin, RESULT_ITER rEnd)
    {
        using namespace boost::accumulators;
        auto mean = extract_result< tag::mean >(acc_);
        rBegin[0]  = replaceRotten(quantile(acc_, quantile_probability = 0.1 ),mean);  
        rBegin[1]  = replaceRotten(quantile(acc_, quantile_probability = 0.25 ),mean); 
        rBegin[2]  = replaceRotten(quantile(acc_, quantile_probability = 0.5 ),mean);  
        rBegin[3] = replaceRotten(quantile(acc_, quantile_probability = 0.75 ),mean); 
        rBegin[4] = replaceRotten(quantile(acc_, quantile_probability = 0.90 ),mean); 
    }

private:

    AccumulatorType acc_;

};

} // end namespace

template<class T>
std::vector<T> rightTailQuantiles(const vigra::MultiArray<1,T> & values, const size_t cacheSize = 1000)
{
    QuantilesRightTailed<T> RightQuantiles{cacheSize};
    
    for( size_t i = 0; i < values.shape(0); ++i )
    {
        RightQuantiles.acc(values(i));
    }
    
    std::vector<T> res(QuantilesRightTailed<T>::NFeatures::value);
    RightQuantiles.result(res.begin(), res.end());
    return res;
}

template<class T>
std::vector<T> leftTailQuantiles(const vigra::MultiArray<1,T> & values, const size_t cacheSize = 1000)
{
    QuantilesLeftTailed<T> LeftQuantiles{cacheSize};
    
    for( size_t i = 0; i < values.shape(0); ++i )
    {
        LeftQuantiles.acc(values(i));
    }
    
    std::vector<T> res(QuantilesLeftTailed<T>::NFeatures::value);
    LeftQuantiles.result(res.begin(), res.end());
    return res;
}
