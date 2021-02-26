/*! \mainpage Concurrency from hell: Prime Pairs
  \author Josh Hudziak
  \date 26/02/2021
  \name Finding Prime Pairs
  \section Using C++ and OpenMP, write a parallel program that counts the number of primes less than n for any number n
  and also find and list all the twin primes (5,7), (11,13) less than n.
  The main objective is to use parallel programming to produce efficient code using multiple threads. The finished program is timed and
  rated while using different amounts of threads. The Max threads for my machine is 6.
  \copyright  License: This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License. To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/.
  \section dep_sec Dependencies
  Linux, G++, fopenmp, gdp debuging and make (makefile), doxygen
  \section install_sec Installation
  Installation can be done in the CLI running Makefile -k
  And Run by using ./prime_parallel_1core in the CLI.
*/
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <chrono>
using namespace std;
#include <omp.h>
/*! \fn define NUM_THREADS derives the number of threads OpenMP will use*/
#define NUM_THREADS 6

/*! \fn void printTP(vector<int>primeNumbers)
  \brief Function to have the vector primepairs out put onto the console

  \param vector<int>primeNumbers this is a vector which holds the prime pairs to be printed

  Trying to use openMP here created a mixed up output has the threads were printing at different times
  thus, 2 pairs would become interwoven. It also reduces efficiency
*/
void printTP(vector<int>primeNumbers){

    int count = 0;

    //    omp_set_num_threads(NUM_THREADS);
//#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < primeNumbers.size() - 1; i++){

        //#pragma omp critical

        if (primeNumbers.at(i+1) - primeNumbers.at(i) == 2){

            cout << "(" << primeNumbers.at(i) << "," << primeNumbers.at(i+1) << "), ";
            count +=1;
        }
    }
    cout << "\nCount of Pairs of Twin Primes: " << count << endl;
}

/*! \fn inline void Eratosethenes_sieve(bool* sieve, int numbers)
    \brief this function will take in a number n and calculate the primes up until that number and add them to the global vector primes
    \param sieve is a boolean that tells us if the multiples of number have been crossed, therefore, no need to check them.
    \param number is the amount of numbers to go through.

    The simplest way to run this algorithm is to use dynamic scheduling built in OpenMP. This scheduling gives the faster
    threads beefier workloads.A loop is split into seperate tasks which are performed by different threads.
    The first loop goes from 2 to the square root of number. And if sieve is unmarked we know it's a prime.
    In the second loop all multiples of number are marked as composite, however this loop shouldn't be made to run parallel,
    break statement cannot run in openMP inside of threaded for loops.
    It was tried regardless and it decreased efficiency by a large margin, so, don't do this.
    Primes can be added to a vector later and output with another function.
*/
inline void Eratosethenes_sieve(bool* sieve, int numbers)
{
    sieve[0] = sieve[1] = 1;
    int numbers_sqrt = (int)(sqrt(numbers));


    omp_set_num_threads(NUM_THREADS);
#pragma omp parallel for schedule(dynamic)
    for (int i = 2; i <= numbers_sqrt; i++) {
        if (!sieve[i]) {

            //    omp_set_num_threads(NUM_THREADS);
//#pragma omp parallel for schedule(dynamic, 1)
            for(int j = i*i; j<= numbers; j += i)
                sieve[j] = 1;
        }
    }
}

/*! \fn int main()
  \brief In main we will run our functions together without any concurrency as
  it is producing segmentation errors while filling the vector with primes.

  Take in a number from the user, start the timer using chrono. Using our function
  Eratosethenes_sieve, we then produce primes into a vector called prime. Next the primes are printed usaing printTP..
  There was no specification for ordering which suits my need for speed perefctly fine.
  Finishing up we display the amount of twin primes, normal primes and the time taken to run.
  In linux the "time ./%" function is used to give cpu and elapsed time so conclusions can be easily drawn on the go.
*/
int main() {


    int numbers = 1e9;
    // cout << "Input an integer:\n";
    //cin >> numbers;

    auto startTime = chrono::high_resolution_clock::now();
    bool* sieve = new bool[numbers+1];
    memset(sieve, 0, numbers + 1);

    Eratosethenes_sieve(sieve, numbers);

    vector<int> prime;

//    omp_set_num_threads(NUM_THREADS);
//#pragma omp parallel for schedule(auto)
    for(int i = 2; i <= numbers; i++)
//#pragma omp critical
        if (!sieve[i]) prime.push_back(i);

    printTP(prime);

    auto endTime = std::chrono::high_resolution_clock::now();
    auto totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    cout << "Parallel Execution Time:" << totalTime << "ms" << endl;
    cout <<"\n Number of Primes: " << prime.size() << "\n";

    return 0;
}
