/*
 * Copyright (C) 1998-2014 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#include <alps/hdf5.hpp>

#include <boost/filesystem.hpp>
#include "gtest/gtest.h"

class my_class {
    public:
        my_class(double v = 0): d(v) {}
        void save(alps::hdf5::archive & ar) const {
            using alps::make_pvp;
            ar << make_pvp("value", d);
        }
        void load(alps::hdf5::archive & ar) { 
            using alps::make_pvp;
            ar >> make_pvp("value", d); 
        }
    private:
        double d;
};

TEST(hdf5, TestingHDF5Misc){

    if (boost::filesystem::exists(boost::filesystem::path("data.h5")))
        boost::filesystem::remove(boost::filesystem::path("data.h5"));

    {
        alps::hdf5::archive ar("data.h5", "w");
        ar << alps::make_pvp("/value", 42);
    }
    
    {
        alps::hdf5::archive ar("data.h5");
        int i;
        ar >> alps::make_pvp("/value", i);
    }

    {
        alps::hdf5::archive ar("data.h5");
        std::string s;
        ar >> alps::make_pvp("/value", s);
    }

    {
        alps::hdf5::archive ar("data.h5", "w");
        std::vector<double> vec(5, 42);
        ar << alps::make_pvp("/path/2/vec", vec);
    }
    
    {
        std::vector<double> vec;
        // fill the vector
        alps::hdf5::archive ar("data.h5");
        ar >> alps::make_pvp("/path/2/vec", vec);
    }

    {
        std::string str("foobar");
        alps::hdf5::archive ar("data.h5", "w");
        ar << alps::make_pvp("/foo/bar", str);
    }
    
    {
        alps::hdf5::archive ar("data.h5");
        std::string str;
        ar >> alps::make_pvp("/foo/bar", str);
    }

    {
        long *d = new long[17];
        // fill the array
        alps::hdf5::archive ar("data.h5", "w");
        ar << alps::make_pvp("/c/array", d, 17);
        delete[] d;
    }

    {
        alps::hdf5::archive ar("data.h5");
        std::size_t size = ar.extent("/c/array")[0];
        long *d = new long[size];
        ar >> alps::make_pvp("/c/array", d, size);
        delete[] d;
    }

    {
        {
                my_class c(42);
                alps::hdf5::archive ar("data.h5", "w");
                ar << alps::make_pvp("/my/class", c);
        }
        {
                my_class c;
                alps::hdf5::archive ar("data.h5");
                ar >> alps::make_pvp("/my/class", c);
        }
    }

    {
        alps::hdf5::archive ar("data.h5", "w"); 
        // the parent of an attribute must exist
        ar.create_group("/foo");
        ar << alps::make_pvp("/foo/@bar", std::string("hello"));
    }

    {
        alps::hdf5::archive ar("data.h5");
        std::string str;
        ar >> alps::make_pvp("/foo/@bar", str);
    }

    boost::filesystem::remove(boost::filesystem::path("data.h5"));
}
int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

