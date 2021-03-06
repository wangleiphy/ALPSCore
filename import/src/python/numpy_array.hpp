/*
 * Copyright (C) 1998-2014 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

/* $Id: pyalea.cpp 3520 2010-04-09 16:49:53Z tamama $ */

#ifndef ALPS_PYTHON_NUMPY_ARRAY
#define ALPS_PYTHON_NUMPY_ARRAY

#include <boost/python.hpp>
#include <numpy/arrayobject.h>
#include <vector>
#include <valarray>

namespace alps { 
  namespace python {
    namespace numpy {

      void import();

      void convert(boost::python::object const & source, std::vector<double> & target);

      boost::python::numeric::array convert(double source);

      boost::python::numeric::array convert(std::vector<double> const & source);

      boost::python::numeric::array convert(std::vector<std::vector<double> > const & source);

      boost::python::numeric::array convert(std::vector<std::vector<std::vector<double> > > const & source);

      
      // for interchanging purpose between numpy array and std::vector
      template <class T>  inline PyArray_TYPES getEnum();
      
      template <>   PyArray_TYPES inline getEnum<double>()              {  return PyArray_DOUBLE;      }
      template <>   PyArray_TYPES inline getEnum<long double>()         {  return PyArray_LONGDOUBLE;  }
      template <>   PyArray_TYPES inline getEnum<int>()                 {  return PyArray_INT;         }
      template <>   PyArray_TYPES inline getEnum<unsigned int>()        {  return PyArray_INT;         }
      template <>   PyArray_TYPES inline getEnum<unsigned short>()      {  return PyArray_INT;         }
      template <>   PyArray_TYPES inline getEnum<long>()                {  return PyArray_LONG;        }
      template <>   PyArray_TYPES inline getEnum<long long>()           {  return PyArray_LONG;        }
      template <>   PyArray_TYPES inline getEnum<unsigned long long>()  {  return PyArray_LONG;        }
      
      template <class T>
      boost::python::numeric::array convert2numpy(T value)
      {
        import();                 // ### WARNING: forgetting this will end up in segmentation fault!
        
        npy_intp arr_size= 1;   // ### NOTE: npy_intp is nothing but just signed size_t
        boost::python::object obj(boost::python::handle<>(PyArray_SimpleNew(1, &arr_size, getEnum<T>())));  // ### NOTE: PyArray_SimpleNew is the new version of PyArray_FromDims
        void *arr_data= PyArray_DATA((PyArrayObject*) obj.ptr());
        memcpy(arr_data, &value, PyArray_ITEMSIZE((PyArrayObject*) obj.ptr()) * arr_size);
        
        return boost::python::extract<boost::python::numeric::array>(obj);
      }
      
      template <class T>
      boost::python::numeric::array convert2numpy(std::vector<T> const& vec)
      {
        alps::python::numpy::import();                 // ### WARNING: forgetting this will end up in segmentation fault!
        
        npy_intp arr_size= vec.size();   // ### NOTE: npy_intp is nothing but just signed size_t
        boost::python::object obj(boost::python::handle<>(PyArray_SimpleNew(1, &arr_size, getEnum<T>())));  // ### NOTE: PyArray_SimpleNew is the new version of PyArray_FromDims
        void *arr_data= PyArray_DATA((PyArrayObject*) obj.ptr());
        memcpy(arr_data, &vec.front(), PyArray_ITEMSIZE((PyArrayObject*) obj.ptr()) * arr_size);
        
        return boost::python::extract<boost::python::numeric::array>(obj);
      }
      
      template <class T>
      boost::python::numeric::array convert2numpy(std::valarray<T> vec)
      {
        import();                 // ### WARNING: forgetting this will end up in segmentation fault!
        
        npy_intp arr_size= vec.size();   // ### NOTE: npy_intp is nothing but just signed size_t
        boost::python::object obj(boost::python::handle<>(PyArray_SimpleNew(1, &arr_size, getEnum<T>())));  // ### NOTE: PyArray_SimpleNew is the new version of PyArray_FromDims
        void *arr_data= PyArray_DATA((PyArrayObject*) obj.ptr());
        memcpy(arr_data, &vec[0], PyArray_ITEMSIZE((PyArrayObject*) obj.ptr()) * arr_size);
        
        return boost::python::extract<boost::python::numeric::array>(obj);
      }
      
      template <class T>
      std::vector<T> convert2vector(boost::python::object arr)
      {
        import();                 // ### WARNING: forgetting this will end up in segmentation fault!
        
        std::size_t vec_size = PyArray_Size(arr.ptr());
        T * data = (T *) PyArray_DATA(arr.ptr());
        
        std::vector<T> vec(vec_size);
        memcpy(&vec.front(),data, PyArray_ITEMSIZE((PyArrayObject*) arr.ptr()) * vec_size);
        return vec;
      }
      
      template <typename T>
      std::valarray<T> convert2valarray(boost::python::object arr)
      {
        import();                 // ### WARNING: forgetting this will end up in segmentation fault!
        
        std::size_t vec_size = PyArray_Size(arr.ptr());
        T * data = (T *) PyArray_DATA(arr.ptr());
        std::valarray<T> vec(vec_size);
        memcpy(&vec[0],data, PyArray_ITEMSIZE((PyArrayObject*) arr.ptr()) * vec_size);
        return vec;
      }
      
    }
  }
}

#include "numpy_array.ipp"

#endif
