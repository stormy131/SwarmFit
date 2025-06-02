#ifndef LINEAR_H
#define LINEAR_H

#include <format>

#include "base.h"
#include "../types.h"
#include "../utils/parallel.h"

struct LinearModel : public BaseModel {
    LinearModel(const size_t dim)
        : BaseModel(dim, std::move(Domains(dim + 1, DOMAIN)))
    {}

    std::string get_slug() const override { return "linear model"; }
    const size_t get_model_dims() const override { return _dim + 1; }
    double predict(const AppTypes::PredictorSet& predictors, const ParamsArray& theta) const override;

private:
    static constexpr auto DOMAIN = std::pair{-10, 10};
};

#endif
