
#include <boost/lexical_cast.hpp>
#include <boost/timer/timer.hpp>
#include <boost/unordered_set.hpp>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include <array_hash_set.hpp>
#include <pool_allocator.hpp>
#include <treap_set.hpp>

#include <google/dense_hash_set>

struct IntGenerator
{
  static void generate(std::vector<int>& v, int count)
  {
    while (count--)
    {
      int x = rand();
      if (x != 0 && x != -1)
        v.push_back(x);
      else
        ++count;
    }
  }
};

struct StringGenerator
{
  static void generate(std::vector<std::string>& v, int count)
  {
    while (count--)
      v.push_back(generate());
  }

  static std::string generate()
  {
    std::ostringstream oss;
    unsigned length = 1 + (rand() % 8);
    for(unsigned i = 0; i != length; ++i)
      oss << static_cast<char>('a' + (rand() % 26));
    return oss.str();
  }
};

template <typename Set>
static void initialize(Set& set);

template <typename Set, typename Generator, typename T>
static double test_inserts();

template <typename Set, typename Generator, typename T>
static double test_find_success();

template <typename Set, typename Generator, typename T>
static double test_find_failure();

template <typename Set, typename Generator, typename T>
static double test_erase_success();

template <typename Set, typename Generator, typename T>
static double test_erase_failure();

template <typename Set, typename Generator, typename T>
static double test_clear();

template <typename Set, typename Generator, typename T>
static double test_mix();

template <typename Set, typename Generator, typename T>
static std::string test_all();

#define TEST_ALL

int main()
{
  srand(time(NULL));
  std::cout << "container,inserts,find(success),find(failure),erase(success),erase(failure),clear" << std::endl;
#ifdef TEST_ALL
  std::cout << "std::set<int>," 
            << test_all<std::set<int>, IntGenerator, int>() 
            << std::endl;
  std::cout << "std::set<int,pool_allocator>,"
            << test_all<std::set<int,std::less<int>,sib::pool_allocator<int> >,
                        IntGenerator, int>()
            << std::endl;
  std::cout << "boost::unordered_set<int>,"
            << test_all<boost::unordered_set<int>, IntGenerator, int>()
            << std::endl;
  /*std::cout << "boost::unordered_set<int,pool_allocator>,"
            << test_all<boost::unordered_set<int,boost::hash<int>,std::equal_to<int>,sib::pool_allocator<int> >, IntGenerator, int>()
            << std::endl;*/
  std::cout << "google::dense_hash_set<int>,"
            << test_all<google::dense_hash_set<int>, IntGenerator, int>()
            << std::endl;
#endif
  std::cout << "sib::array_hash_set<int>,"
            << test_all<sib::array_hash_set<int>, IntGenerator, int>()
            << std::endl;
  std::cout << "sib::treap_set<int>,"
            << test_all<sib::treap_set<int>, IntGenerator, int>()
            << std::endl;
  std::cout << "sib::treap_set<int,pool_allocator>,"
            << test_all<sib::treap_set<int,sib::comparator<int>,sib::standard_random_generator,sib::pool_allocator<int> >, IntGenerator, int>()
            << std::endl;
#ifdef TEST_ALL
  std::cout << "std::set<string>," 
            << test_all<std::set<std::string>, StringGenerator, std::string>() 
            << std::endl;
  std::cout << "std::set<string,pool_allocator>,"
            << test_all<std::set<std::string,std::less<std::string>,sib::pool_allocator<std::string> >,
                        StringGenerator, std::string>()
            << std::endl;
  std::cout << "boost::unordered_set<std::string>,"
            << test_all<boost::unordered_set<std::string>, StringGenerator, std::string>()
            << std::endl;
  std::cout << "google::dense_hash_set<string>,"
            << test_all<google::dense_hash_set<std::string>, StringGenerator, std::string>()
            << std::endl;
#endif
  std::cout << "sib::array_hash_set<string>,"
            << test_all<sib::array_hash_set<std::string>, StringGenerator, std::string>()
            << std::endl;
  std::cout << "sib::treap_set<string>,"
            << test_all<sib::treap_set<std::string>, StringGenerator, std::string>()
            << std::endl;
  std::cout << "sib::treap_set<string,pool_allocator>,"
            << test_all<sib::treap_set<std::string,sib::comparator<std::string>,sib::standard_random_generator,sib::pool_allocator<std::string> >, StringGenerator, std::string>()
            << std::endl;
  return 0;
}

template <typename Set, typename Generator, typename T>
static std::string test_all()
{
  std::string result;
  result += boost::lexical_cast<std::string>(test_inserts<Set,Generator,T>());
  result += ",";
  result += boost::lexical_cast<std::string>(test_find_success<Set,Generator,T>());
  result += ",";
  result += boost::lexical_cast<std::string>(test_find_failure<Set,Generator,T>());
  /*result += ",";
  result += boost::lexical_cast<std::string>(test_erase_success<Set,Generator,T>());
  result += ",";
  result += boost::lexical_cast<std::string>(test_erase_failure<Set,Generator,T>());*/
  result += ",";
  result += boost::lexical_cast<std::string>(test_clear<Set,Generator,T>());
  //result += ",";
  //result += boost::lexical_cast<std::string>(test_mix<Set,Generator,T>());
  return result;
}

template <typename Set, typename Generator, typename T>
static double test_inserts()
{
  static unsigned const InsertIterations = 10000;
  static unsigned const InsertCount = 1000;

  Set set;
  initialize<Set>(set);
  boost::timer::cpu_timer timer;
  double total_nanos = 0;
  std::vector<T> data;
  for(unsigned i = 0; i != InsertIterations; ++i)
  {
    data.clear();
    set.clear();
    Generator::generate(data, InsertCount);
    timer.start();
    for(typename std::vector<T>::const_iterator iter = data.begin();
        iter != data.end();
        ++iter)
    {
      set.insert(*iter);
    }
    total_nanos += timer.elapsed().wall;
  }
  return (unsigned)(total_nanos / (InsertIterations * InsertCount));
}

template <typename Set, typename Generator, typename T>
static double test_find_success()
{
  static unsigned const Iterations = 10000;
  static unsigned const Count = 1000;

  Set set;
  initialize<Set>(set);
  boost::timer::cpu_timer timer;
  double total_nanos = 0;
  std::vector<T> data;
  int found = 0;
  for(unsigned i = 0; i != Iterations; ++i)
  {
    data.clear();
    set.clear();
    Generator::generate(data, Count);
    for(typename std::vector<T>::const_iterator iter = data.begin();
        iter != data.end();
        ++iter)
    {
      set.insert(*iter);
    }
    timer.start();
    for(typename std::vector<T>::const_iterator iter = data.begin();
        iter != data.end();
        ++iter)
    {
      if (set.find(*iter) != set.end())
        ++found;
    }
    total_nanos += timer.elapsed().wall;
  }
  if (found == 123) // never
    std::cout << "yeah right\n";
  return (unsigned)(total_nanos / (Iterations * Count));
}

template <typename Set, typename Generator, typename T>
static double test_find_failure()
{
  static unsigned const Iterations = 10000;
  static unsigned const Count = 1000;

  Set set;
  initialize<Set>(set);
  boost::timer::cpu_timer timer;
  double total_nanos = 0;
  std::vector<T> data;
  int found = 0;
  for(unsigned i = 0; i != Iterations; ++i)
  {
    data.clear();
    set.clear();
    Generator::generate(data, Count);
    for(typename std::vector<T>::const_iterator iter = data.begin();
        iter != data.end();
        ++iter)
    {
      set.insert(*iter);
    }
    data.clear();
    Generator::generate(data, Count);
    timer.start();
    for(typename std::vector<T>::const_iterator iter = data.begin();
        iter != data.end();
        ++iter)
    {
      if (set.find(*iter) != set.end())
        ++found;
    }
    total_nanos += timer.elapsed().wall;
  }
  if (found == 123) // never
    std::cout << "yeah right\n";
  return (unsigned)(total_nanos / (Iterations * Count));
}

template <typename Set, typename Generator, typename T>
static double test_erase_success()
{
  static unsigned const Iterations = 10000;
  static unsigned const Count = 1000;

  Set set;
  initialize<Set>(set);
  boost::timer::cpu_timer timer;
  double total_nanos = 0;
  std::vector<T> data;
  for(unsigned i = 0; i != Iterations; ++i)
  {
    data.clear();
    set.clear();
    Generator::generate(data, Count);
    for(typename std::vector<T>::const_iterator iter = data.begin();
        iter != data.end();
        ++iter)
    {
      set.insert(*iter);
    }
    timer.start();
    for(typename std::vector<T>::const_iterator iter = data.begin();
        iter != data.end();
        ++iter)
    {
      set.erase(*iter);
    }
    total_nanos += timer.elapsed().wall;
  }
  return (unsigned)(total_nanos / (Iterations * Count));
}

template <typename Set, typename Generator, typename T>
static double test_erase_failure()
{
  static unsigned const Iterations = 10000;
  static unsigned const Count = 1000;

  Set set;
  initialize<Set>(set);
  boost::timer::cpu_timer timer;
  double total_nanos = 0;
  std::vector<T> data;
  for(unsigned i = 0; i != Iterations; ++i)
  {
    data.clear();
    set.clear();
    Generator::generate(data, Count);
    for(typename std::vector<T>::const_iterator iter = data.begin();
        iter != data.end();
        ++iter)
    {
      set.insert(*iter);
    }
    data.clear();
    Generator::generate(data, Count);
    timer.start();
    for(typename std::vector<T>::const_iterator iter = data.begin();
        iter != data.end();
        ++iter)
    {
      set.erase(*iter);
    }
    total_nanos += timer.elapsed().wall;
  }
  return (unsigned)(total_nanos / (Iterations * Count));
}

template <typename Set, typename Generator, typename T>
static double test_clear()
{
  static unsigned const Iterations = 10000;
  static unsigned const Count = 1000;

  Set set;
  initialize<Set>(set);
  boost::timer::cpu_timer timer;
  double total_nanos = 0;
  std::vector<T> data;
  for(unsigned i = 0; i != Iterations; ++i)
  {
    data.clear();
    set.clear();
    Generator::generate(data, Count);
    for(typename std::vector<T>::const_iterator iter = data.begin();
        iter != data.end();
        ++iter)
    {
      set.insert(*iter);
    }
    timer.start();
    set.clear();
    total_nanos += timer.elapsed().wall;
  }
  return (unsigned)(total_nanos / (Iterations * Count));
}

template <typename Set, typename Generator, typename T>
static double test_mix()
{
  static unsigned const Iterations = 10000;
  static unsigned const Count = 1000;

  Set set;
  initialize<Set>(set);
  boost::timer::cpu_timer timer;
  double total_nanos = 0;
  std::vector<T> data;
  for(unsigned i = 0; i != Iterations; ++i)
  {
    data.clear();
    Generator::generate(data, Count);
    timer.start();
    for(typename std::vector<T>::const_iterator iter = data.begin();
        iter != data.end();
        ++iter)
    {
      set.insert(*iter);
    }
    for(typename std::vector<T>::const_iterator iter = data.begin();
        iter != data.end();
        ++iter)
    {
      set.erase(*iter);
    }
    total_nanos += timer.elapsed().wall;
  }
  return (unsigned)(total_nanos / (Iterations * Count));
}

template <typename Set>
static void initialize(Set& set)
{
}

template <>
void initialize<google::dense_hash_set<int> >(google::dense_hash_set<int>& set)
{
  set.set_empty_key(0);
  set.set_deleted_key(-1);
}

template <>
void initialize<google::dense_hash_set<std::string> >(google::dense_hash_set<std::string>& set)
{
  set.set_empty_key("");
  set.set_deleted_key("?");
}

