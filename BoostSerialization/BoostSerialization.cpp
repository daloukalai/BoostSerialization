#include<iostream>
#include<assert.h>
#include<fstream>
#include<boost/archive/text_iarchive.hpp>
#include<boost/archive/text_oarchive.hpp>

using namespace std;

namespace boost
{
    namespace serialization 
    {
        class access;
    }
}

class Obj 
{
public:
  // Serialization expects the object to have a default constructor
  Obj() : d1_(-1), d2_(false) {}
  Obj(int d1, bool d2) : d1_(d1), d2_(d2) {}
  bool operator==(const Obj& o) const {
    return d1_ == o.d1_ && d2_ == o.d2_;
  }
private:
  int d1_;
  bool d2_;

  // Allow serialization to access non-public data members.
  friend class boost::serialization::access;

  template<typename Archive>
  void serialize(Archive& ar, const unsigned version) {
    ar & d1_ & d2_; // Simply serialize the data members of Obj
  }
};

void main()
{
    cout << "Boost Serialization" << endl;
    const char* fileName = "C:\\BOOST_SERIALIZED_OBJ.txt";

    // Create some objects
    const Obj o1(-2, false);
    const Obj o2;
    const Obj o3(21, true);
    const Obj* const p1 = &o1;

    // Save data
    {
        // Create an output archive
        ofstream ofs(fileName);
        boost::archive::text_oarchive ar(ofs);

        // Write data
        ar & o1 & o2 & o3 & p1;
    }

    // Restore data
    Obj restored_o1;
    Obj restored_o2;
    Obj restored_o3;
    Obj* restored_p1;
    {
        // Create and input archive
        std::ifstream ifs(fileName);
        boost::archive::text_iarchive ar(ifs);

        // Load data
        ar & restored_o1 & restored_o2 & restored_o3 & restored_p1;
    }
    // Make sure we restored the data exactly as it was saved
    assert(restored_o1 == o1);
    assert(restored_o2 == o2);
    assert(restored_o3 == o3);
    assert(restored_p1 != p1);
    assert(restored_p1 == &restored_o1);
}