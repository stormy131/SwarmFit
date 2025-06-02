#ifndef RANDOM_H
#define RANDOM_H
#include <random>

template<typename ArgType>
ArgType random_value(ArgType min, ArgType max, std::mt19937& gen) {
    std::uniform_real_distribution<ArgType> distr(min, max);
    return distr(gen);
}

#endif
