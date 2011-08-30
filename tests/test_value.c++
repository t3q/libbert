#include <bert/value.hpp>
#include <bert/parse.hpp>

#include <boost/range/iterator_range.hpp>
#include <boost/test/unit_test.hpp>

#include <vector>
#include <string>
#include <iostream>

using namespace bert;
using namespace std;
using namespace boost;

BOOST_AUTO_TEST_SUITE( test_value )

BOOST_AUTO_TEST_CASE( test_value_default ) {
  value v;
  BOOST_CHECK(!v.is_small_integer());
  BOOST_CHECK(!v.is_integer());
  BOOST_CHECK(!v.is_integral_type());
  BOOST_CHECK(!v.is_float());
  BOOST_CHECK(!v.is_atom());
  BOOST_CHECK(!v.is_small_tuple());
  BOOST_CHECK(!v.is_large_tuple());
  BOOST_CHECK(!v.is_tuple());
  BOOST_CHECK(v.is_nil());
  BOOST_CHECK(!v.is_string());
  BOOST_CHECK(!v.is_list());
  BOOST_CHECK(!v.is_binary());
  BOOST_CHECK(!v.is_small_big());
  BOOST_CHECK(!v.is_large_big());
  BOOST_CHECK(!v.is_big());
  BOOST_CHECK_EQUAL(v.get_type(), NIL_EXT);
}

BOOST_AUTO_TEST_CASE( test_value_nil ) {
  value v(NIL_EXT, nil());
  BOOST_CHECK(!v.is_small_integer());
  BOOST_CHECK(!v.is_integer());
  BOOST_CHECK(!v.is_integral_type());
  BOOST_CHECK(!v.is_float());
  BOOST_CHECK(!v.is_atom());
  BOOST_CHECK(!v.is_small_tuple());
  BOOST_CHECK(!v.is_large_tuple());
  BOOST_CHECK(!v.is_tuple());
  BOOST_CHECK(v.is_nil());
  BOOST_CHECK(!v.is_string());
  BOOST_CHECK(!v.is_list());
  BOOST_CHECK(!v.is_binary());
  BOOST_CHECK(!v.is_small_big());
  BOOST_CHECK(!v.is_large_big());
  BOOST_CHECK(!v.is_big());
  BOOST_CHECK_EQUAL(v.get_type(), NIL_EXT);
}

BOOST_AUTO_TEST_CASE( test_value_small_int ) {
  value v(SMALL_INTEGER_EXT, byte_t(3));
  BOOST_CHECK(v.is_small_integer());
  BOOST_CHECK(!v.is_integer());
  BOOST_CHECK(v.is_integral_type());
  BOOST_CHECK(!v.is_float());
  BOOST_CHECK(!v.is_atom());
  BOOST_CHECK(!v.is_small_tuple());
  BOOST_CHECK(!v.is_large_tuple());
  BOOST_CHECK(!v.is_tuple());
  BOOST_CHECK(!v.is_nil());
  BOOST_CHECK(!v.is_string());
  BOOST_CHECK(!v.is_list());
  BOOST_CHECK(!v.is_binary());
  BOOST_CHECK(!v.is_small_big());
  BOOST_CHECK(!v.is_large_big());
  BOOST_CHECK(!v.is_big());
  BOOST_CHECK_EQUAL(v.get_type(), SMALL_INTEGER_EXT);
  BOOST_CHECK_EQUAL(v.get_small_integer(), byte_t(3));
}

BOOST_AUTO_TEST_CASE( test_value_int ) {
  value v(INTEGER_EXT, boost::int32_t(3));
  BOOST_CHECK(!v.is_small_integer());
  BOOST_CHECK(v.is_integer());
  BOOST_CHECK(v.is_integral_type());
  BOOST_CHECK(!v.is_float());
  BOOST_CHECK(!v.is_atom());
  BOOST_CHECK(!v.is_small_tuple());
  BOOST_CHECK(!v.is_large_tuple());
  BOOST_CHECK(!v.is_tuple());
  BOOST_CHECK(!v.is_nil());
  BOOST_CHECK(!v.is_string());
  BOOST_CHECK(!v.is_list());
  BOOST_CHECK(!v.is_binary());
  BOOST_CHECK(!v.is_small_big());
  BOOST_CHECK(!v.is_large_big());
  BOOST_CHECK(!v.is_big());
  BOOST_CHECK_EQUAL(v.get_type(), INTEGER_EXT);
  BOOST_CHECK_EQUAL(v.get_integer(), boost::int32_t(3));
}

// ...

BOOST_AUTO_TEST_CASE( test_value_term_to_str ) {


  // X = term_to_binary({test_atom,{"test_string",12345},"test_string1","test_string2",54321}).
  byte_t srcBuffer1[] = {131,104,5,100,0,9,116,101,115,116,95,97,116,111,109,104,2,107,0,11,116,101,
                            115,116,95,115,116,114,105,110,103,98,0,0,48,57,107,0,12,116,101,115,116,95,
                            115,116,114,105,110,103,49,107,0,12,116,101,115,116,95,115,116,114,105,110,
                            103,50,98,0,0,212,49};

  // X = term_to_binary({a,1,123456,3.14,[1,2, "ala ma kota", {tuple, x},[]],"kot ma ale", <<123,254,1,0,0>>}).
  byte_t srcBuffer2[] = {131,104,7,100,0,1,97,97,1,98,0,1,226,64,99,51,46,49,52,48,48,48,48,48,48,48,
                         48,48,48,48,48,48,49,50,52,51,52,101,43,48,48,0,0,0,0,0,108,0,0,0,5,97,1,97,
                         2,107,0,11,97,108,97,32,109,97,32,107,111,116,97,104,2,100,0,5,116,117,112,
                         108,101,100,0,1,120,106,106,107,0,10,107,111,116,32,109,97,32,97,108,101,109,
                         0,0,0,5,123,254,1,0,0};

  // Check 1
  iterator_range<byte_t const *> range1(srcBuffer1, srcBuffer1 + sizeof(srcBuffer1));
  vector<value> v = parse(range1, parse_version);
  BOOST_CHECK(v.size() > 0);
  string tmp1 = v[0].str();
  cout << "TERM TO STRING: " << tmp1 << "\r\n";
  BOOST_CHECK(tmp1.compare("{test_atom,{\"test_string\",12345},\"test_string1\",\"test_string2\",54321}") == 0);


  // Check 2
  iterator_range<byte_t const *> range2(srcBuffer2, srcBuffer2 + sizeof(srcBuffer2));
  v = parse(range2, parse_version);
  BOOST_CHECK(v.size() > 0);
  string tmp2 = v[0].str();
  cout << "TERM TO STRING: " << tmp2 << "\r\n";
  BOOST_CHECK(tmp1.compare("{a,1,123456,3.14,[1,2,\"ala ma kota\",{tuple,x},[]],\"kot ma ale\",<<123,254,1,0,0>>}") == 0);
}

BOOST_AUTO_TEST_SUITE_END()
