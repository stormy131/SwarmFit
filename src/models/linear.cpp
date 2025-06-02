#include "linear.h"
#include <numeric>

double LinearModel::predict(const AppTypes::PredictorSet& predictors, const ParamsArray& theta) const {
    validate_dims(predictors, _dim);
    validate_dims(theta, get_model_dims());

    const ParamsArray X = append_bias(predictors);
    double res{};
    for (size_t i = 0; i < X.size(); ++i) {
        res += X[i] * theta[i];
    }

    return res;
}