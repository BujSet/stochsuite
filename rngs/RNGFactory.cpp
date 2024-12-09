#include "RNGFactory.hpp"
#include "Taus88.hpp"
#include "Taus113.hpp"
#include <string>
#include <memory>

RNGFactory::RNGFactory() {}

RNGFactory::~RNGFactory() {}

std::unique_ptr<RNGBase> RNGFactory::createRNG(const std::string type) {
    std::string t88("Taus88");
    std::string t113("Taus113");
    if (type == t88) {
        return std::unique_ptr<RNGBase>(new Taus88());
    } else if (type == t113) {
        return std::unique_ptr<RNGBase>(new Taus113());
    }else {
        return nullptr;
    }
}
