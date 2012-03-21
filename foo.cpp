
#include <boost/timer/timer.hpp>
#include <google/dense_hash_map>
#include <iostream>
#include <map>
#include <unordered_map>

#include "bucket_hash_map.hpp"
#include "simple_hash_map.hpp"
#include "vector_hash_map.hpp"

using namespace sib;

#define COUNT 10000000

#define USE_SIB
#define LOOKUPS

//typedef double KEY;
//static const KEY EMPTY_KEY = -1.0;

typedef std::size_t KEY;
static const KEY EMPTY_KEY = 0xffffffffffffffff;

static const KEY TARGET = (KEY)COUNT;

int main(void)
{
#ifdef USE_SIB
  sib::bucket_hash_map<KEY,std::size_t> map;
  //sib::simple_hash_map<KEY,std::size_t> map;
  //sib::vector_hash_map<KEY,std::size_t> map;
#else
  //std::map<KEY,std::size_t> map;
  //std::unordered_map<KEY,std::size_t> map;
  google::dense_hash_map<KEY,std::size_t> map;
  map.set_empty_key(EMPTY_KEY);
#endif
  std::size_t total(1);
  
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

#ifndef LOOKUPS
  boost::timer::cpu_timer timer;
  timer.start();
#endif
  for(KEY ii = 0; ii != TARGET; ++ii) {
    map[ii] = ii;
  }
#ifndef LOOKUPS
  timer.stop();
#else
  boost::timer::cpu_timer timer;
  timer.start();
  for(KEY ii = 0; ii != TARGET; ++ii) {
    total += map[ii];
  }
  timer.stop();
#endif
  std::cout << (timer.elapsed().wall / ((double)COUNT)) << " ns\n";
#ifdef USE_SIB
  std::cout << "avg bucket depth: " << map.avg_bucket_depth() << std::endl;
#endif
  return 0;
}

