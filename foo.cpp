
#include <boost/timer/timer.hpp>
#include <google/dense_hash_map>
#include <iostream>
#include <map>
#include <unordered_map>

#include "simple_hash_map.hpp"
#include "vector_hash_map.hpp"

using namespace sib;

//#define COUNT 10000000
#define COUNT 3

#define USE_SIB

typedef double KEY;
static const KEY EMPTY_KEY = -1.0;
static const KEY TARGET = (KEY)COUNT;

int main(void)
{
#ifdef USE_SIB
  //sib::hash_map<KEY,std::size_t> map;
  sib::vector_hash_map<KEY,std::size_t> map;
#else
  //std::map<std::size_t,std::size_t> map;
  //std::unordered_map<std::size_t,std::size_t> map;
  google::dense_hash_map<KEY,std::size_t> map;
  map.set_empty_key(EMPTY_KEY);
#endif
  
  for(KEY ii = 0; ii < TARGET; ++ii) {
    map[ii] = ii;
  }
#ifdef USE_SIB
  std::cout << "avg bucket depth: " << map.avg_bucket_depth() << std::endl;
#endif
  map.clear();
#ifdef USE_SIB
  std::cout << "avg bucket depth: " << map.avg_bucket_depth() << std::endl;
#endif

  boost::timer::cpu_timer timer;
  timer.start();
  for(KEY ii = 0; ii != TARGET; ++ii) {
    map[ii] = ii;
  }
  timer.stop();
  std::cout << (timer.elapsed().wall / ((double)COUNT)) << " ns\n";
#ifdef USE_SIB
  std::cout << "avg bucket depth: " << map.avg_bucket_depth() << std::endl;
#endif
  return 0;
}

