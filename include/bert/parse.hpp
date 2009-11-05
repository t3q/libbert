#ifndef LIBBERT_PARSE_HPP
#define LIBBERT_PARSE_HPP

#include "scan.hpp"
#include "value.hpp"
#include "complex_scan.hpp"

namespace bert {
  enum parse_flag_t {
    parse_everything = 1u, // parse the complete range
    parse_version = 2u     // parse version tag
  };

  inline parse_flag_t operator|(parse_flag_t lhs, parse_flag_t rhs) {
    return static_cast<parse_flag_t>(static_cast<unsigned>(lhs) |
                                     static_cast<unsigned>(rhs));
  }

  inline parse_flag_t operator&(parse_flag_t lhs, parse_flag_t rhs) {
    return static_cast<parse_flag_t>(static_cast<unsigned>(lhs) |
                                     static_cast<unsigned>(rhs));
  }

  inline parse_flag_t operator~(parse_flag_t f) {
    return static_cast<parse_flag_t>(~static_cast<unsigned>(f));
  }

  namespace detail {
    template<class Range>
    std::vector<value> parse(Range &r, parse_flag_t flags, unsigned n = ~0) {
      std::vector<value> ret;
      if(!r) {
        return ret;
      }
      if(n == 0) {
        n = 1;
      }
      for(; n != 0 && r; --n) {
        type_t const t = get_type(r);
        switch(t) {
        case SMALL_INTEGER_EXT:
          ret.push_back(value(t, get_small_integer(r)));
          break;
        case INTEGER_EXT:
          ret.push_back(value(t, get_integer(r)));
          break;
        case FLOAT_EXT:
          ret.push_back(value(t, get_float(r)));
          break;
        case ATOM_EXT:
          ret.push_back(value(t, get_atom(r)));
          break;
        case SMALL_TUPLE_EXT: {
#if 0
          if(flags & parse_complex) {
            // TODO
          }
#endif
          byte_t const size = get_small_tuple_size(r);
          ret.push_back(value(t, parse(r, flags, size)));
        }
          break;
        case LARGE_TUPLE_EXT: {
#if 0
          if(flags & parse_complex) {
            // TODO
          }
#endif
          boost::uint32_t const size = get_large_tuple_size(r);
          ret.push_back(value(t, parse(r, flags, size)));
        }
          break;
        case NIL_EXT:
          ret.push_back(value());
          break;
        case STRING_EXT:
          ret.push_back(value(t, get_string(r)));
          break;
        case LIST_EXT: {
          // TODO handle Tail!
          boost::uint32_t const size = get_list_size(r);
          /* not optimal: collecting into a vector and then constructing a list
             from that */
          std::vector<value> tmp = parse(r, flags, size);
          ret.push_back(value(t, std::list<value>(tmp.begin(), tmp.end())));
        }
          break;
        case BINARY_EXT:
          ret.push_back(value(t, get_binary(r)));
          break;
#ifndef LIBBERT_NO_EXTENSION
        case X_NEW_FLOAT_EXT:
          ret.push_back(value(FLOAT_EXT, x_get_new_float(r)));
          break;
#endif
        default:
          throw bert_exception("unkown type");
        };
      }
      return ret;
    }
  }

  /**
   * Parse BERT Data.
   */
  template<typename Range>
  std::vector<value> parse(Range r,
                           parse_flag_t flags = parse_everything|parse_complex)
  {
    if(flags & parse_version) {
      get_version(r);
    }
    return detail::parse(r, flags, flags & parse_everything ? ~0 : 1);
  }
}

#endif
