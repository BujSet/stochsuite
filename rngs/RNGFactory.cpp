#include "RNGFactory.hpp"
#include "Taus88.hpp"
#include <string>
#include <memory>

RNGFactory::RNGFactory() {}

RNGFactory::~RNGFactory() {}

std::unique_ptr<RNGBase> RNGFactory::createRNG(const std::string& type) {
    std::string s("Taus88");
    if (type == s) {
        return std::unique_ptr<RNGBase>(new Taus88());
    } else {
        return nullptr;
    }
}
