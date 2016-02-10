/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */


#ifndef PUNCH_LANG_UTIL_HPP
#define PUNCH_LANG_UTIL_HPP

#include <memory>
#include <boost/throw_exception.hpp>

inline void boost::throw_exception(std::exception const &e) { };

namespace punch {
  namespace lang {

    template<typename T, typename ...Args>
    std::unique_ptr<T> make_unique(Args &&...args) {
      return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    template<typename T>
    std::unique_ptr<T> make_unique() {
      return std::unique_ptr<T>{};
    }

    template<typename T, std::size_t sz>
    inline T *begin(T (&array)[sz]) { return array; }

    template<typename T, std::size_t sz>
    inline T *end(T (&array)[sz]) { return array + sz; }

    struct EnumClassHash {
      template<typename T>
      std::size_t operator()(T t) const {
        return static_cast<std::size_t>(t);
      }
    };

    template<class S, class T>
    T *of_type(std::unique_ptr<S> &val) {
      T *p = static_cast<T *>(&*val);
      return p;
    }

    template<typename S, typename T>
    T const *as_type(const S *other) {
      if (const T *p = static_cast<T const *>(other)) {
        return p;
      }
      else {
        return nullptr;
      }
    }
  }
}

#endif //PUNCH_LANG_UTIL_HPP
