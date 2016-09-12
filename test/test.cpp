#include <sstream>
#include <iostream>

#include "BTC.h"

int main() {

    std::cout << "Store values to stream" << std::endl;
    // Create new Compound object.
    BTC::BTagCompoundPtr test(new BTC::BTagCompound());
    // Set some values.
    test->setInt("integer",1);
    test->setFloat("float",1.4f);
    test->setDouble("double",1.3452e-10);
    // Create another tag.
    BTC::BTagCompoundPtr tag(new BTC::BTagCompound());
    // Add the tag to the "outer" tag under name "inner_tag".
    test->setTag("inner_tag",tag);
    // Set values in the inner tag.
    tag->setString("a_string","some string");
    double* doubarr = new double[20];
    for (int i=0; i<20; ++i) {
        doubarr[i] = i*0.13413723472374e-15;
        std::cout << doubarr[i] << ' ';
    }
    std::cout << std::endl;
    tag->passDoubleArray("doubarr",doubarr,20);
    tag->setShort("bla",20);
    // Print the whole object.
    std::cout << *test << std::endl;
    // Serialize to stream.
    std::stringstream ss;
    test->serialize(ss);
    //ss.clear();

    std::cout << "Load values from stream" << std::endl;
    // Create another compound object.
    BTC::BTagCompoundPtr test1(new BTC::BTagCompound());
    // Deserialize from stream.
    test1->deserialize(ss);
    // Print the values.
    std::cout << *test1 << std::endl;
    BTC::BTagCompoundPtr tag1 = test1->getTag<BTC::BTagCompound>("inner_tag");
    std::cout << tag1->getValue<unsigned short>("bla") << std::endl;
    size_t len;
    doubarr = tag1->getArray<double>("doubarr",len);
    for (int i=0; i<len; ++i) std::cout << doubarr[i] << ' ';
    std::cout << std::endl;
    BTC::DOUBLE_T doub = test1->getValue<BTC::DOUBLE_T>("double");
    std::cout << doub << std::endl;

    return 0;
}
