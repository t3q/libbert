#include <bert/format.hpp>

#include <boost/range/iterator_range.hpp>
#include <boost/test/unit_test.hpp>

#include <vector>
#include <algorithm>
#include <iostream>
#include <exception>
using namespace bert;
using namespace std;
using namespace boost;

BOOST_AUTO_TEST_SUITE( format_test )

BOOST_AUTO_TEST_CASE( format_test_iter ) {
  vector<byte_t> buf;
  back_insert_iterator< vector<byte_t> > j = format_integer(10, back_inserter(buf));


}

bool compare_arrays(byte_t *a1, int a1Size, byte_t *a2, int a2Size) {
  // size check should be done before call
  for(int i = 0; i < a1Size; i++) {
    //cout << (int)*(a1+i) << " == " << (int)*(a2+i) << endl;
    if(*(a1+i) != *(a2+i))
      return false;
  }
  return true;
}

BOOST_AUTO_TEST_CASE( format_test ) {
  //  R14B03

  // X = term_to_binary({test_atom,{"test_string",12345},"test_string1","test_string2",54321}).
  byte_t srcBuffer1[255] = {131,104,5,100,0,9,116,101,115,116,95,97,116,111,109,104,2,107,0,11,116,101,
                            115,116,95,115,116,114,105,110,103,98,0,0,48,57,107,0,12,116,101,115,116,95,
                            115,116,114,105,110,103,49,107,0,12,116,101,115,116,95,115,116,114,105,110,
                            103,50,98,0,0,212,49};
  int srcLength1 = 71;

  // X = term_to_binary("").
  byte_t srcBuffer2[255] = {131,106};
  int srcLength2 = 2;

  // X = term_to_binary(3.14).
  byte_t srcBuffer3[255] = {131,99,51,46,49,52,48,48,48,48,48,48,48,48,48,48,48,48,48,49,50,52,51,52,101,
                            43,48,48,0,0,0,0,0};
  int srcLength3 = 33;

  byte_t buffer[255];
  buffer[0] = 131;

  iterator_range<byte_t *> range(buffer, buffer + sizeof(buffer));
  iterator_range<byte_t *>::iterator i;
  int length = 0;

  // check 1
  i = range.begin();
  i = format_small_tuple_size(5, ++i);
  i = format_atom(string("test_atom"), i);
  i = format_small_tuple_size(2, i);
  i = format_string(string("test_string"), i);
  i = format_integer(12345, i);
  i = format_string(string("test_string1"), i);
  i = format_string(string("test_string2"), i);
  i = format_integer(54321, i);  
  length = &(*i) - buffer;
  BOOST_CHECK(srcLength1 == length);
  BOOST_CHECK(compare_arrays(srcBuffer1, srcLength1, buffer, length));

  // check 2 - We should get NIL_EX instead of STRING_EXT when string is empty
  i = range.begin();
  i = format_string(string(""), ++i);
  length = &(*i) - buffer;
  BOOST_CHECK(srcLength2 == length);
  BOOST_CHECK(compare_arrays(srcBuffer2, srcLength2, buffer, length));

  // check 3
  i = range.begin();
  i = format_float(3.14, ++i);
  length = &(*i) - buffer;
  BOOST_CHECK(srcLength3 == length);
  BOOST_CHECK(compare_arrays(srcBuffer3, srcLength3, buffer, length)); 
}

BOOST_AUTO_TEST_SUITE_END()
