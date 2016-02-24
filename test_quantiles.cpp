#include <random>
#include <iostream>
#include <chrono>

#include "quantiles_boost.hxx"
#include "quantiles_vigra.hxx"
#include "quantiles_exact.hxx"

void test_quantiles(const size_t NValues, const size_t NReps)
{
    using namespace vigra;
    typedef std::chrono::nanoseconds time_type;
    // get random generator
    std::random_device r;
    std::mt19937 generator(r());
    std::uniform_real_distribution<double> distr(0.0, 10000.0);

    // repititions
    for( size_t rep = 0; rep < NReps; ++rep )
    {
        // fill vigra multi array with NValues random values
        MultiArray<1, double> test_values{ Shape1(NValues) };
        for( size_t ii = 0; ii < NValues; ++ii )
        {
            double val = distr(generator);
            test_values(ii) = val;
            //std::cout << val << std::endl;
        }
        
        // get the quantiles and time
        // exact quantiles
        auto start_ex = std::chrono::system_clock::now();
        std::vector<double> q_ex = exactQuantiles<double>(test_values); 
        auto t_ex = std::chrono::duration_cast<time_type>(
                std::chrono::system_clock::now() - start_ex );
        
        // vigra quantiles
        auto start_vi= std::chrono::system_clock::now();
        std::vector<double> q_vi = vigraQuantiles<double>(test_values); 
        auto t_vi = std::chrono::duration_cast<time_type>(
                std::chrono::system_clock::now() - start_vi );
        
        // boost right quantiles
        auto start_bo_r= std::chrono::system_clock::now();
        std::vector<double> q_bo_r = rightTailQuantiles<double>(test_values); 
        auto t_bo_r = std::chrono::duration_cast<time_type>(
                std::chrono::system_clock::now() - start_bo_r );
        
        // boost left quantiles
        auto start_bo_l= std::chrono::system_clock::now();
        std::vector<double> q_bo_l = leftTailQuantiles<double>(test_values); 
        auto t_bo_l = std::chrono::duration_cast<time_type>(
                std::chrono::system_clock::now() - start_bo_l );
        
        std::cout << t_ex.count() << std::endl;
        std::cout << t_vi.count() << std::endl;
        std::cout << t_bo_r.count() << std::endl;
        std::cout << t_bo_l.count() << std::endl;
        
        std::cout << "Quantiles Exact: 0.1 = " << q_ex[0] << ", 0.25 = " << q_ex[1] << ", 0.5 = " << q_ex[2];
        std::cout << ", 0.75 = " << q_ex[3] << ", 0.9 = " << q_ex[4] <<  std::endl;
        
        std::cout << "Quantiles Vigra: 0.1 = " << q_vi[0] << ", 0.25 = " << q_vi[1] << ", 0.5 = " << q_vi[2];
        std::cout << ", 0.75 = " << q_vi[3] << ", 0.9 = " << q_vi[4] <<  std::endl;
        
        std::cout << "Quantiles Boost right: 0.1 = " << q_bo_r[0] << ", 0.25 = " << q_bo_r[1] << ", 0.5 = " << q_bo_r[2];
        std::cout << ", 0.75 = " << q_bo_r[3] << ", 0.9 = " << q_bo_r[4] <<  std::endl;
        
        std::cout << "Quantiles Boost left: 0.1 = " << q_bo_l[0] << ", 0.25 = " << q_bo_l[1] << ", 0.5 = " << q_bo_l[2];
        std::cout << ", 0.75 = " << q_bo_l[3] << ", 0.9 = " << q_bo_l[4] <<  std::endl;
        
    }
    

}


int main()
{
    test_quantiles(1000, 1);
}
