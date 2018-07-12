#ifndef _CYZPP_UTILITY_H__
#define _CYZPP_UTILITY_H__
#include "common.h"
#include <type_traits>

CYZPP_BEGIN

namespace compile_time_using {

// >>> compile time sequence index
template <::std::size_t... I>
struct sequence_index {};

// general
template <::std::size_t S, class Index, ::std::size_t E>
struct make_sequence_index_imp {};

// iteration
template <::std::size_t S, ::std::size_t... Index, ::std::size_t E>
struct make_sequence_index_imp<S, sequence_index<Index...>, E> {
  typedef
      typename make_sequence_index_imp<S + 1, sequence_index<Index..., S>, E>::type
          type;
};

// termination
template <::std::size_t E, ::std::size_t... Index>
struct make_sequence_index_imp<E, sequence_index<Index...>, E> {
  typedef sequence_index<Index..., E> type;
};

template <::std::size_t S, ::std::size_t E>
struct make_sequence_index {
  typedef typename make_sequence_index_imp<S, sequence_index<>, E>::type type;
};

}  // namespace compile_time

CYZPP_END

#endif  // !_CYZPP_UTILITY_H__