/*
 * Copyright (C) 1998-2014 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_NGS_DETAIL_GET_NUMPY_TYPE_HPP
#define ALPS_NGS_DETAIL_GET_NUMPY_TYPE_HPP

#include <alps/ngs/config.hpp>

#if !defined(ALPS_HAVE_PYTHON)
    #error numpy is only available if python is enabled
#endif

#include <alps/ngs/boost_python.hpp>
#include <alps/ngs/detail/numpy_import.hpp>

#include <complex>

#define ALPS_NGS_FOREACH_NATIVE_NUMPY_TYPE(CALLBACK)                                                                                            \
    CALLBACK(bool)                                                                                                                              \
    CALLBACK(char)                                                                                                                              \
    CALLBACK(signed char)                                                                                                                       \
    CALLBACK(unsigned char)                                                                                                                     \
    CALLBACK(short)                                                                                                                             \
    CALLBACK(unsigned short)                                                                                                                    \
    CALLBACK(int)                                                                                                                               \
    CALLBACK(unsigned)                                                                                                                          \
    CALLBACK(long)                                                                                                                              \
    CALLBACK(unsigned long)                                                                                                                     \
    CALLBACK(long long)                                                                                                                         \
    CALLBACK(unsigned long long)                                                                                                                \
    CALLBACK(float)                                                                                                                             \
    CALLBACK(double)                                                                                                                            \
    CALLBACK(long double)                                                                                                                       \
    CALLBACK(std::complex<float>)                                                                                                               \
    CALLBACK(std::complex<double>)                                                                                                              \
    CALLBACK(std::complex<long double>)

namespace alps {
    namespace detail {
        #define ALPS_NGS_DECL_NUMPY_TYPE(T)                                                                                                     \
            ALPS_DECL int get_numpy_type(T);
        ALPS_NGS_FOREACH_NATIVE_NUMPY_TYPE(ALPS_NGS_DECL_NUMPY_TYPE)
        #undef ALPS_NGS_DECL_NUMPY_TYPE
    }
}

#endif
