Examining the differences in quantile calculations in vigra and boost.

# Results

For all tests, random double values in the range 0, 10000 are generated.
All tests were conducted in 3 variants: With input in unsorted, sorted in ascending and sorted in
descending oreder.
The differences are computed with respect to an exact calculation of the quantiles, which was
tested against some numpy results and should yield the right results.

## Initial Results

Results with the initial parameters for the accumulators:
* Boost: Cache Size 1000
* Vigra: Auto-Range Histogram with 10 bins 

**NValues = 10, NReps = 1000**

Unsorted input

|       | Diff - Vigra  | Diff - Boost right    | Diff - Boost left     |        
|-------|--------------:|----------------------:|----------------------:|
| Q-0.1 | -267.0        | 91.2                  | -820.9                |
| Q-0.25| -215.2        | -226.0                | - 226.0               |
| Q-0.5 | -164.7        | 446.2                 | - 446.2               |
| Q-0.75| 203.5         | 218.9                 | 218.9                 |
| Q-0.9 | 54.9          | 805.9                 | -89.5                 |

Sorted in ascendig order

|       | Diff - Vigra  | Diff - Boost right    | Diff - Boost left     |        
|-------|--------------:|----------------------:|----------------------:|
| Q-0.1 | -238.7        | 86.3                  | -776.5                |
| Q-0.25| -215.6        | -231.7                | -231.7                |
| Q-0.5 | -168.0        | 445.1                 | -445.1                |
| Q-0.75| 225.5         | 229.9                 | 229.9                 |
| Q-0.9 | 59.8          | 823.0                 | -91.4                 |

Sorted in descendig order

|       | Diff - Vigra  | Diff - Boost right    | Diff - Boost left     |        
|-------|--------------:|----------------------:|----------------------:|
| Q-0.1 | -265.2        | 90.6                  | -815.3                |
| Q-0.25| -208.5        | -234.3                | -234.3                |
| Q-0.5 | -174.4        | 452.6                 | -452.6                |
| Q-0.75| 214.9         | 233.6                 | 233.6                 |
| Q-0.9 | 56.7          | 773.8                 | -86.0                 |

For a small number of values all three methods do not perform well. 
We see, that vigra understimates the lower quantiles and overestimates the higher quantiles.
Boost right overestimates all but the 0.25-quantile, Boost right does exactly the opposite.
None of the methods is affected by ordering of the inpuy data.
Overall, the vigra results are more accurate. (All three show systematic errors).

**NValues = 100, NReps = 1000**

Unsorted input

|       | Diff - Vigra  | Diff - Boost right    | Diff - Boost left     |        
|-------|--------------:|----------------------:|----------------------:|
| Q-0.1 | -26.7         | 9.9                   | -88.8                 |
| Q-0.25| -29.7         | 24.3                  | -72.9                 |
| Q-0.5 | 2.9           | 49.2                  | -49.2                 |
| Q-0.75| 30.4          | 77.6                  | -25.9                 |
| Q-0.9 | 20.1          | 87.3                  | -9.7                  |

Sorted in ascendig order

|       | Diff - Vigra  | Diff - Boost right    | Diff - Boost left     |        
|-------|--------------:|----------------------:|----------------------:|
| Q-0.1 | -35.3         | 10.4                  | -93.4                 |
| Q-0.25| -26.9         | 25.1                  | -75.4                 |
| Q-0.5 | -3.1          | 49.8                  | -49.8                 |
| Q-0.75| 26.1          | 76.3                  | -25.4                 |
| Q-0.9 | 29.2          | 88.4                  | -9.8                  |

Sorted in descendig order

|       | Diff - Vigra  | Diff - Boost right    | Diff - Boost left     |        
|-------|--------------:|----------------------:|----------------------:|
| Q-0.1 | -28.3         | 10.0                  | -90.2                 |
| Q-0.25| -21.5         | 24.7                  | -74.2                 |
| Q-0.5 | 5.5           | 48.0                  | -48.0                 |
| Q-0.75| 27.8          | 76.1                  | -25.4                 |
| Q-0.9 | 27.3          | 91.4                  | -10.2                 |

Performance of all methods is better.
However the systematics of errors does not change (Apart from the fact that Boost right now overestimates all quantiles and Boost left underestimates all).
Again overall vigra performs best and the ordering is irrelevant for all three methods.


**NValues = 1000, NReps = 1000**

Unsorted input

|       | Diff - Vigra  | Diff - Boost right    | Diff - Boost left     |        
|-------|--------------:|----------------------:|----------------------:|
| Q-0.1 | -4.9          | 1.0                   | -8.8                  |
| Q-0.25| -4.1          | 2.4                   | -7.2                  |
| Q-0.5 | -0.9          | 5.0                   | -5.0                  |
| Q-0.75| 1.2           | 8.0                   | -2.7                  |
| Q-0.9 | 2.9           | 9.4                   | -1.0                  |

All methods are pretty accurate, same systematics of the errors as before.
I don't report the sorted results, because again there is no difference.


**NValues = 10000, NReps = 1000**

Unsorted input

|       | Diff - Vigra  | Diff - Boost right    | Diff - Boost left     |        
|-------|--------------:|----------------------:|----------------------:|
| Q-0.1 | -0.7          | 3998.9                | 3998.9                |
| Q-0.25| -1.1          | 2497.9                | 2497.9                |
| Q-0.5 | -0.4          | -0.4                  | -0.4                  |
| Q-0.75| 0.7           | -2498.4               | -2498.4               |
| Q-0.9 | 0.6           | -3998.2               | -3998.2               |

Vigra is pretty accurate, Boost methods fail completely (I am pretty sure that
some "CacheOverflow" happens and all values are replcaed with the mean...).
Again now differnce for different sorting.


Conclusion: Both Methods perform bad for small numbers of values.
Both perform good for intermediate sample sizes.
Vigra performs good for large sample sizes, Boost totally fails.
** I am well aware, that the Boost performs most probably depends critically on the cache size.
However I don't want to spend any more time fiddling with this and will continue with only the vigra method. **


## Adjusting number of Bins

Looking for a good heuristics to set a reasonable number of bins, I came up with the following:
NumberBins = ceil( pow(NValues,1./2.5) ), with minimum 2 and maximum 64.
(Haven't put much thought into this. 
Experiments showed that something that grows stronger cbrt, but slower than
sqrt yields best results. Ceil because otherwise bincount too small for small N).

Comparison with bins fixed to 10:

**NValues = 10, NReps = 10000**

|       | Diff - Fixed  | Diff - Adaptive       |        
|-------|--------------:|----------------------:|
| Q-0.1 | -261.5        | 64.6                  |
| Q-0.25| -212.1        | 22.2                  |
| Q-0.5 | -178.1        | -14.3                 |
| Q-0.75| 215.0         | -14.6                 |
| Q-0.9 | 65.1          | -41.6                 |

**NValues = 100, NReps = 10000**

|       | Diff - Fixed  | Diff - Adaptive       |        
|-------|--------------:|----------------------:|
| Q-0.1 | -27.6         | 1.7                   |
| Q-0.25| -26.5         | -18.7                 |
| Q-0.5 |  0.4          | 1.3                   |
| Q-0.75|  27.4         | 19.4                  |
| Q-0.9 |  28.0         | 2.6                   |

**NValues = 1000, NReps = 10000**

|       | Diff - Fixed  | Diff - Adaptive       |        
|-------|--------------:|----------------------:|
| Q-0.1 | -3.5          | -3.1                  |
| Q-0.25| -2.6          | -2.4                  |
| Q-0.5 | -0.4          | -0.4                  |
| Q-0.75| 3.1           | 2.5                   |
| Q-0.9 | 3.4           | 2.6                   |


**NValues = 10000, NReps = 10000**

|       | Diff - Fixed  | Diff - Adaptive       |        
|-------|--------------:|----------------------:|
| Q-0.1 | -0.3          | -0.3                  |
| Q-0.25| -0.4          | -0.2                  |
| Q-0.5 | -0.1          | -0.1                  |
| Q-0.75| 0.1           | 0.2                   |
| Q-0.9 | 0.4           | 0.4                   |

For small sample sizes, the adaptive bins perform better (but still not very good.)
For large samples sizes, both perform equally good..
