#include <fstream>
#include <iostream>

#include "BTC.h"

int main() {

    // Store values to disk
    BTC::BTagCompound test;
    test.setValue<uint32_t>("integer",1);
    test.setValue<float>("float",1.4f);
    test.setValue<double>("double",1.3452e-10);
    BTC::BTagCompoundPtr tag = BTC::BTagCompoundPtr::fromObject(new BTC::BTagCompound());
    test.setTag("othertag",tag);
    tag->setValue<std::string>("ein_string","irgendein quatsch den man eben so schreibt =)");
    double* doubarr = new double[20];
    for (int i=0; i<20; ++i) doubarr[i] = i*0.13413723472374e-15;
    for (int i=0; i<20; ++i) std::cout << doubarr[i] << ' ';
    std::cout << std::endl;
    tag->putArray<double>("doubarr",doubarr,20);
    tag->setValue<uint16_t>("bla",20);
    std::cout << test << std::endl;
    std::fstream file_out("./temp.dat",std::fstream::out);
    test.serialize(file_out);
    file_out.close();

    // Load values
    BTC::BTagCompound test1;
    std::fstream file_in("./temp.dat",std::fstream::in);
    test1.deserialize(file_in);
    file_in.close();
    std::cout << test1 << std::endl;
    std::cout << test1.getValue<double>("double") << std::endl;
    BTC::BTagCompoundPtr tag1 = test1.getTag<BTC::BTagCompound>("othertag");
    std::cout << *tag1 << std::endl;
    std::cout << tag1->getValue<uint16_t>("bla") << std::endl;
    size_t len;
    doubarr = tag1->getArray<double>("doubarr",len);
    for (int i=0; i<len; ++i) std::cout << doubarr[i] << ' ';
    std::cout << std::endl;

    return 0;
}
