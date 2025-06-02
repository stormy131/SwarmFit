#ifndef FIT_RUNNER_H
#define FIT_RUNNER_H

#include <memory>

#include "types.h"
#include "csv_handler.h"
#include "models/base.h"
#include "evaluators/base.h"
#include "pso/optimizer.h"

class FitRunner {
    using ParamsArray = AppTypes::ParamsArray;
    using Dataset = AppTypes::Dataset;
    using Model = std::shared_ptr<BaseModel>;
    using Evaluator = std::shared_ptr<BaseEvaluator>;
    using OptimizedFunction = std::function<double(const ParamsArray)>;

    const Model _model;
    const Evaluator _evaluator;
    const Dataset& _data;

    // Argument value type is required for TArg derivation in traits
    const OptimizedFunction eval_fit = [this](const ParamsArray& theta) -> double {
        return (-1) * _evaluator->evaluate(_data, theta);
    };
    using TargetContext = FunctionContext<decltype(eval_fit)>;

    Optimizer<TargetContext> setup_optimizer(int max_steps);

public:
    FitRunner(Model model, Evaluator evaluator, const Dataset& data)
        : _model(std::move(model)), _evaluator(evaluator), _data(data) {}

    ParamsArray fit(double min_delta, int n_tolerance, int max_steps);
};

#endif
