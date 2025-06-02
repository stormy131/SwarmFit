#ifndef TYPES_H
#define TYPES_H

#include <memory>
#include <utility>
#include <vector>

namespace AppTypes {
    using PredictorSet = std::vector<double>;
    using DataPoint = std::pair<PredictorSet, double>;
    using Dataset = std::vector<DataPoint>;

    // Model parameters & domains
    using ParamsArray = std::vector<double>;
    using ParamDomain = std::pair<double, double>;
    using Domains = std::vector<ParamDomain>;

    // Log file entry (row). Model identifier, (training iteration, error value)
    using LogEntry = std::pair<std::string&, std::pair<int, double>>;
}

#endif
