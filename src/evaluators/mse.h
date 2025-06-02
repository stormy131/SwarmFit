#ifndef MSE_H
#define MSE_H

#include <memory>
#include "base.h"

struct MSEEvaluator : public BaseEvaluator {
    MSEEvaluator(const Model model) : BaseEvaluator(model) {}
    double evaluate(const DataSet& data, const ModelParams& theta) const override;
};

#endif
