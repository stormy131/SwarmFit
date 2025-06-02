#include "sine.h"
#include <cmath>

double SineModel::predict(const AppTypes::PredictorSet& predictors, const ParamsArray& theta) const {
    validate_dims(predictors, _dim);
    validate_dims(theta, get_model_dims());

    double sum = 0.0;
    for (size_t i = 0; i < predictors.size(); ++i) {
        const double a = theta[3 * i];
        const double b = theta[3 * i + 1];
        const double c = theta[3 * i + 2];
        sum += a * std::sin(b * predictors[i] + c);
    }

    return sum + theta.back();
}