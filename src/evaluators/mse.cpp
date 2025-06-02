#include "mse.h"

double MSEEvaluator::evaluate(const DataSet& data, const ModelParams& theta) const {
    double sum_squared_error = 0.0;

    for (const auto& [input, target] : data) {
        const double prediction = _model->predict(input, theta);
        const double error = prediction - target;
        sum_squared_error += error * error;
    }

    return sum_squared_error / data.size();
}