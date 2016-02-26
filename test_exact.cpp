#include <iostream>
#include <cassert>

#include "quantiles_exact.hxx"

// quantiles_expected generated with numpy in true_quantiles.py
void test_exact()
{
    using namespace vigra;

    // make test data equivalent to:
    //arrays = [np.arange(0,10), np.arange(0,11), np.arange(0,1000), # simple
    //        np.arange(0,1e6), np.array([2.3,4.7,1.8,3.3,6.7,7.8,9.4,17.8,3.5])]
    std::vector<MultiArray<1, double> > test_arrays;
    
    MultiArray<1, double> test_values1{ Shape1(10) };
    for( size_t i = 0; i < 10; i++)
        test_values1[i] = i;
    test_arrays.push_back(test_values1);
    
    MultiArray<1, double> test_values2{ Shape1(11) };
    for( size_t i = 0; i < 11; i++)
        test_values2[i] = i;
    test_arrays.push_back(test_values2);
    
    MultiArray<1, double> test_values3{ Shape1(1000) };
    for( size_t i = 0; i < 1000; i++)
        test_values3[i] = i;
    test_arrays.push_back(test_values3);
    
    MultiArray<1, double> test_values4{ Shape1(1e6) };
    for( size_t i = 0; i < 1e6; i++)
        test_values4[i] = i;
    test_arrays.push_back(test_values4);
    
    MultiArray<1, double> test_values5{ Shape1(9) };
    test_values5(0) = 2.3;
    test_values5(1) = 4.7;
    test_values5(2) = 1.8;
    test_values5(3) = 3.3;
    test_values5(4) = 6.7;
    test_values5(5) = 7.8;
    test_values5(6) = 9.4;
    test_values5(7) = 17.8;
    test_values5(8) = 3.5;
    test_arrays.push_back(test_values5);

    // expected quantiles
    // results from true_quantiles.py
    //0.9 2.25 4.5 6.75 8.1
    //1.0 2.5 5.0 7.5 9.0
    //99.9 249.75 499.5 749.25 899.1
    //99999.9 249999.75 499999.5 749999.25 899999.1
    //2.2 3.3 4.7 7.8 11.08

    std::vector<std::vector<double> > quantiles_expected;
    quantiles_expected.push_back(std::vector<double>{{0.9, 2.25, 4.5, 6.75, 8.1}});
    quantiles_expected.push_back(std::vector<double>{{1.0, 2.5, 5.0, 7.5, 9.0}});
    quantiles_expected.push_back(std::vector<double>{{99.9, 249.75, 499.5, 749.25, 899.1}});
    quantiles_expected.push_back(std::vector<double>{{99999.9, 249999.75, 499999.5, 749999.25, 899999.1}});
    quantiles_expected.push_back(std::vector<double>{{2.2, 3.3, 4.7, 7.8, 11.08}});
    
    for( size_t i = 0; i < test_arrays.size(); i++)
    {
        MultiArray<1, double> test_values = test_arrays[i];
        std::vector<double> expected = quantiles_expected[i];
        // reulting quantiles
        std::vector<double> quantiles_obtained = exactQuantiles<double>(test_values);
        assert(expected.size() == quantiles_obtained.size());
        if( expected == quantiles_obtained )
        {
            std::cout << "Check " << i << " passed" << std::endl;
        }
        else
        {
            std::cout << "Check " << i << " failed" << std::endl;
            for(size_t ii = 0; ii < expected.size(); ii++)
            {
                std::cout <<  "Quantile " << ii << std::endl;
                std::cout << "Expected: " << expected[ii] << " Obtained: " << quantiles_obtained[ii] << std::endl;
            }
        }
    }
    

}

int main()
{
    test_exact();
}
