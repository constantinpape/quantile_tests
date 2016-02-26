#include <random>
#include <iostream>
#include <chrono>
#include <functional>
#include <cassert>

#include "quantiles_boost.hxx"
#include "quantiles_vigra.hxx"
#include "quantiles_exact.hxx"

void test_quantiles(const size_t NValues, const size_t NReps, const int sort = 0 )
{
    using namespace vigra;
    typedef std::chrono::nanoseconds time_type;
    // get random generator
    std::random_device r;
    std::mt19937 generator(r());
    std::uniform_real_distribution<double> distr(0.0, 10000.0);

    const size_t NQuants = 5;

    // array to save the differences in results
    MultiArray<2, double>  diff_vi{ Shape2(NReps, NQuants) };
    MultiArray<2, double> diff_bor{ Shape2(NReps, NQuants) };
    MultiArray<2, double> diff_bol{ Shape2(NReps, NQuants) };

    size_t time_ex = 0;
    size_t time_vi = 0;
    size_t time_bor = 0;
    size_t time_bol = 0;

    // repititions
    for( size_t rep = 0; rep < NReps; ++rep )
    {
        // fill vigra multi array with NValues random values
        MultiArray<1, double> test_values{ Shape1(NValues) };
        for( size_t ii = 0; ii < NValues; ++ii )
        {
            double val = distr(generator);
            test_values(ii) = val;
        }
        
        // see if data should be sorted
        // 0 -> dont sort, 1 -> sort, 2 -> sort in reverse order
        switch(sort)
        {
            case 0:
                break;
            case 1:
                std::sort(test_values.begin(), test_values.end());
                break;
            case 2:
                std::sort( test_values.begin(), test_values.end(), std::greater<double>() );
                break;
            default:
                std::cout << "No option for sort = " << sort;
                std::cout << " so we don't sort the data." << std::endl;
                break;
        }
            
        // get the quantiles and time
        // exact quantiles
        auto start_ex = std::chrono::system_clock::now();
        std::vector<double> q_ex = exactQuantiles<double>(test_values); 
        auto t_ex = std::chrono::duration_cast<time_type>(
                std::chrono::system_clock::now() - start_ex );
        time_ex += t_ex.count();
        
        // vigra quantiles
        auto start_vi= std::chrono::system_clock::now();
        std::vector<double> q_vi = vigraQuantiles<double>(test_values, 2); 
        auto t_vi = std::chrono::duration_cast<time_type>(
                std::chrono::system_clock::now() - start_vi );
        time_vi += t_vi.count();
        
        // boost right quantiles
        auto start_bo_r= std::chrono::system_clock::now();
        std::vector<double> q_bo_r = rightTailQuantiles<double>(test_values); 
        auto t_bo_r = std::chrono::duration_cast<time_type>(
                std::chrono::system_clock::now() - start_bo_r );
        time_bor += t_bo_r.count();
        
        // boost left quantiles
        auto start_bo_l= std::chrono::system_clock::now();
        std::vector<double> q_bo_l = leftTailQuantiles<double>(test_values); 
        auto t_bo_l = std::chrono::duration_cast<time_type>(
                std::chrono::system_clock::now() - start_bo_l );
        time_bol += t_bo_l.count();
        
        // calculate the differences to the exact quantiles
        for( size_t q = 0; q < NQuants; q++)
        {
            diff_vi(rep, q)  = q_vi[q]   - q_ex[q]; 
            diff_bor(rep, q) = q_bo_r[q] - q_ex[q];
            diff_bol(rep, q) = q_bo_l[q] - q_ex[q];
        }

        // debug output
        /*
        
        std::cout << "Quantiles Exact: 0.1 = " << q_ex[0] << ", 0.25 = " << q_ex[1] << ", 0.5 = " << q_ex[2];
        std::cout << ", 0.75 = " << q_ex[3] << ", 0.9 = " << q_ex[4] <<  std::endl;
        
        std::cout << "Quantiles Vigra: 0.1 = " << q_vi[0] << ", 0.25 = " << q_vi[1] << ", 0.5 = " << q_vi[2];
        std::cout << ", 0.75 = " << q_vi[3] << ", 0.9 = " << q_vi[4] <<  std::endl;
        
        std::cout << "Quantiles Boost right: 0.1 = " << q_bo_r[0] << ", 0.25 = " << q_bo_r[1] << ", 0.5 = " << q_bo_r[2];
        std::cout << ", 0.75 = " << q_bo_r[3] << ", 0.9 = " << q_bo_r[4] <<  std::endl;
        
        std::cout << "Quantiles Boost left: 0.1 = " << q_bo_l[0] << ", 0.25 = " << q_bo_l[1] << ", 0.5 = " << q_bo_l[2];
        std::cout << ", 0.75 = " << q_bo_l[3] << ", 0.9 = " << q_bo_l[4] <<  std::endl;
        */

    }
    std::cout << "Results for " << NReps << " repetitions of " << NValues << " values:" << std::endl;
    std::cout << "Sorting: " << sort << std::endl;
    std::cout << "Calculation times:" << std::endl;
    
    std::cout << "Exact: " << time_ex << std::endl;
    std::cout << "Vigra: " << time_vi << std::endl;
    std::cout << "Boost Right: " << time_bor << std::endl;
    std::cout << "Boost Left: " << time_bol << std::endl;

    std::cout << "Quantiles-differences:" << std::endl;
    for( size_t q = 0; q < NQuants; q++)
    {
        std::cout << "Quantile: " << q << std::endl;
        auto quant_diffs_vi  = diff_vi.bindAt(1,q);
        auto quant_diffs_bor = diff_bor.bindAt(1,q);
        auto quant_diffs_bol = diff_bol.bindAt(1,q);
        assert(quant_diffs_vi.shape(0) == NReps);
        double diff_vi = std::accumulate( quant_diffs_vi.begin(), quant_diffs_vi.end(), 0. );
        diff_vi /= NReps;
        double diff_bor = std::accumulate( quant_diffs_bor.begin(), quant_diffs_bor.end(), 0. );
        diff_bor /= NReps;
        double diff_bol = std::accumulate( quant_diffs_bol.begin(), quant_diffs_bol.end(), 0. );
        diff_bol /= NReps;
        std::cout << "Difference Vigra: " << diff_vi << std::endl;
        std::cout << "Difference Boost Right: " << diff_bor << std::endl;
        std::cout << "Difference Boost Left: " << diff_bol << std::endl;
    }
}


void test_adaptivebins(const size_t NValues, const size_t NReps)
{
    using namespace vigra;
    typedef std::chrono::nanoseconds time_type;
    // get random generator
    std::random_device r;
    std::mt19937 generator(r());
    std::uniform_real_distribution<double> distr(0.0, 10000.0);

    const size_t NQuants = 5;

    // array to save the differences in results
    MultiArray<2, double> diff_vi_fixed{ Shape2(NReps, NQuants) };
    MultiArray<2, double> diff_vi_adapt{ Shape2(NReps, NQuants) };

    // repititions
    for( size_t rep = 0; rep < NReps; ++rep )
    {
        // fill vigra multi array with NValues random values
        MultiArray<1, double> test_values{ Shape1(NValues) };
        for( size_t ii = 0; ii < NValues; ++ii )
        {
            double val = distr(generator);
            test_values(ii) = val;
        }
        
        std::vector<double> q_ex = exactQuantiles<double>(test_values); 
        
        size_t n_adapt = std::ceil( std::pow(NValues, 1./2.5) );
        size_t bin_min = 2;
        size_t bin_max = 64;
        n_adapt = std::max(bin_min, std::min(n_adapt, bin_max) );
        std::vector<double> q_vi_fixed = vigraQuantiles<double>(test_values, 10); 
        std::vector<double> q_vi_adapt = vigraQuantiles<double>(test_values, n_adapt);
        
        // calculate the differences to the exact quantiles
        for( size_t q = 0; q < NQuants; q++)
        {
            diff_vi_fixed(rep, q) = q_vi_fixed[q] - q_ex[q]; 
            diff_vi_adapt(rep, q) = q_vi_adapt[q] - q_ex[q];
        }

    }
    std::cout << "Results for " << NReps << " repetitions of " << NValues << " values:" << std::endl;
    
    std::cout << "Quantiles-differences:" << std::endl;
    for( size_t q = 0; q < NQuants; q++)
    {
        std::cout << "Quantile: " << q << std::endl;
        auto quant_diffs_vi_fixed  = diff_vi_fixed.bindAt(1,q);
        auto quant_diffs_vi_adapt  = diff_vi_adapt.bindAt(1,q);
        assert(quant_diffs_vi_fixed.shape(0) == NReps);
        double diff_vi_fixed = std::accumulate( quant_diffs_vi_fixed.begin(), quant_diffs_vi_fixed.end(), 0. );
        diff_vi_fixed /= NReps;
        double diff_vi_adapt = std::accumulate( quant_diffs_vi_adapt.begin(), quant_diffs_vi_adapt.end(), 0. );
        diff_vi_adapt /= NReps;
        std::cout << "Difference Vigra - Fixed: " << diff_vi_fixed << std::endl;
        std::cout << "Difference Vigra - Adapt: " << diff_vi_adapt << std::endl;
    }
}


int main()
{
    test_adaptivebins(10000, 10000);
}
