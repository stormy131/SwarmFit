#ifndef SINE_H
#define SINE_H

#include <format>

#include "base.h"
#include "../types.h"

inline constexpr auto DOMAIN = std::pair{-30, 30};

struct SineModel : public BaseModel{
    SineModel(const size_t dim)
        : BaseModel( dim, Domains(dim, DOMAIN))
    {}

    std::string get_slug() const override { return "sine model"; }
    const size_t get_model_dims() const override { return 3*_dim + 1; }
    double predict(const AppTypes::PredictorSet& predictors, const ParamsArray& theta) const override;

private:
    static constexpr auto DOMAIN = std::pair{-30, 30};
};

#endif
