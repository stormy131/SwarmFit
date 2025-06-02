#ifndef BASE_EVALUATOR_H
#define BASE_EVALUATOR_H

#include "../types.h"
#include "../models/base.h"

struct BaseEvaluator {
protected:
    using Model = std::shared_ptr<BaseModel>;
    using ModelParams = AppTypes::ParamsArray;
    using DataSet = AppTypes::Dataset;

    const Model _model;
    BaseEvaluator(const Model model) : _model(model) {}

public:
    virtual ~BaseEvaluator() = default;
    virtual double evaluate(const DataSet& data, const ModelParams& theta) const = 0;
};

#endif
