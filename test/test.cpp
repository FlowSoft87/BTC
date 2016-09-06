#include <sstream>
#include <iostream>

#include "BTC.h"

int main() {

    std::cout << "Store values to stream" << std::endl;
    // Create new Compound object.
    BTC::BTagCompoundPtr test(new BTC::BTagCompound());
    // Set some values.
    test->setValue<uint32_t>("integer",1);
    test->setValue<float>("float",1.4f);
    test->setValue<double>("double",1.3452e-10);
    // Create another tag.
    BTC::BTagCompoundPtr tag(new BTC::BTagCompound());
    // Add the tag to the "outer" tag under name "inner_tag".
    test->setTag("inner_tag",tag);
    // Set values in the inner tag.
    tag->setValue<std::string>("a_string","some string");
    double* doubarr = new double[20];
    for (int i=0; i<20; ++i) {
        doubarr[i] = i*0.13413723472374e-15;
        std::cout << doubarr[i] << ' ';
    }
    std::cout << std::endl;
    tag->putArray<double>("doubarr",doubarr,20);
    tag->setValue<uint16_t>("bla",20);
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
    std::cout << test1->getValue<double>("double") << std::endl;
    BTC::BTagCompoundPtr tag1 = test1->getTag<BTC::BTagCompound>("inner_tag");
    std::cout << *tag1 << std::endl;
    std::cout << tag1->getValue<uint16_t>("bla") << std::endl;
    size_t len;
    doubarr = tag1->getArray<double>("doubarr",len);
    for (int i=0; i<len; ++i) std::cout << doubarr[i] << ' ';
    std::cout << std::endl;

    return 0;
}
