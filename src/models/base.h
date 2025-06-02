#ifndef BASE_MODEL_H
#define BASE_MODEL_H

#include <iostream>
#include <format>
#include "../types.h"

struct BaseModel {
protected:
    using ParamsArray = AppTypes::ParamsArray;
    using ParamDomain = AppTypes::ParamDomain;
    using Domains = AppTypes::Domains;
    using PredictorSet = AppTypes::PredictorSet;

    // Expected dimensionality of the input data
    size_t _dim;
    const Domains _param_domains;

    BaseModel(const size_t dim, const Domains&& domains)
        : _dim(dim), _param_domains(domains)
    {}

    void validate_dims(const ParamsArray& target, const size_t expected_dim) const {
        if (target.size() != expected_dim) {
            throw std::invalid_argument("[" + get_slug() + std::format(
            "] invalid input vector size. Expected {}, received {}",
                expected_dim,
                target.size()
            ));
        }
    }

    static ParamsArray append_bias(const AppTypes::PredictorSet& predictors) {
        const size_t dim = predictors.size();
        ParamsArray X(dim + 1);

        std::copy(predictors.begin(), predictors.end(), X.begin());
        X[dim] = 1;

        return X;
    }

public:
    virtual ~BaseModel() = default;
    virtual double predict(const PredictorSet& X, const ParamsArray& theta) const = 0;
    virtual std::string get_slug() const = 0;
    virtual const size_t get_model_dims() const = 0;

    const Domains& get_param_domains() const {
        return _param_domains;
    }
};

#endif
