#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <vector>
#include <sstream>

#include "../types.h"

inline AppTypes::DataPoint make_data_point(const std::vector<double>& values) {
    AppTypes::PredictorSet predictors(values.begin(), values.end() - 1);
    return {std::move(predictors), values.back()};
}

template<typename ArgType>
std::string vector_to_string(const std::vector<ArgType>& vector) {
    std::ostringstream result;
    for(auto&& e : vector) {
        result << e << " ";
    }

    return result.str();
}

#endif
