#include <omp.h>
#include <Rcpp.h>


// [[Rcpp::export]]
void rcpp_hello_()
{
  int tid, nthreads;
  
  #pragma omp parallel default(shared) private(tid, nthreads)
  {
    tid = omp_get_thread_num();
    nthreads = omp_get_num_threads();
    
    Rcpp::Rcout << "Hello from thread " << tid << " of " << nthreads << std::endl;
  }
}



// [[Rcpp::export]]
double rcpp_sum_(Rcpp::NumericVector x)
{
  double ret = 0;
  
  #pragma omp parallel for default(shared) reduction(+:ret)
  for (int i=0; i<x.size(); i++)
    ret += x[i];
  
  return ret;
}



// [[Rcpp::export]]
Rcpp::NumericMatrix rcpp_sweep_(Rcpp::NumericMatrix x, Rcpp::NumericVector vec)
{
  Rcpp::NumericMatrix ret(x.nrow(), x.ncol());
  
  #pragma omp parallel for default(shared)
  for (int j=0; j<x.ncol(); j++)
  {
    #pragma omp simd
    for (int i=0; i<x.nrow(); i++)
      ret(i, j) = x(i, j) - vec(i);
  }
  
  return ret;
}



// [[Rcpp::export]]
int rcpp_primesbelow_(const int n)
{
  int isprime;
  int nprimes = 1;
  
  #pragma omp parallel for private(isprime) reduction(+:nprimes)
  for (int i=3; i<=n; i+=2)
  {
    isprime = 1;
    
    for (int j=3; j<i; j+=2)
    {
      if (i%j == 0)
      {
        isprime = 0;
        break;
      }
    }
    
    if (isprime) nprimes++;
  }
  
  return nprimes;
}

