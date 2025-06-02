#ifndef FUNC_TRAITS_HPP
#define FUNC_TRAITS_HPP

#include <vector>
#include <utility>
#include <stdexcept>
#include <random>


template <typename LambdaFunctor>
struct func_traits : func_traits<decltype(&LambdaFunctor::operator())> {};

template <typename ClassType, typename TReturn, typename TArg>
struct func_traits<TReturn (ClassType::*)(std::vector<TArg>) const> {
    using return_type      = TReturn;
    using arg_type         = TArg;

    static constexpr bool is_vector_wrapper = true;
};

template <typename ClassType, typename TReturn, typename TArg>
struct func_traits<TReturn (ClassType::*)(std::vector<TArg>&) const> {
    using return_type      = TReturn;
    using arg_type         = TArg;

    static constexpr bool is_vector_wrapper = true;
};

template <typename ClassType, typename TReturn, typename TArg, typename... TRest>
struct func_traits<TReturn (ClassType::*)(TArg, TRest...) const> {
    using return_type      = TReturn;
    using arg_type         = TArg;

    static constexpr bool is_vector_wrapper = false;
};


template <typename EvalFunc>
class FunctionContext {
public:
    using traits          = func_traits<EvalFunc>;
    using ArgType         = typename traits::arg_type;
    using ReturnType      = typename traits::return_type;
    using ArgDomain       = std::pair<ArgType, ArgType>;
    using ArgDistribution = std::uniform_real_distribution<ArgType>;
    using StateVector     = std::vector<ArgType>;

    FunctionContext(const EvalFunc& func, const size_t arity, const std::vector<ArgDomain>& func_domains)
        : eval_func(func), arity(arity), domains(func_domains) {}

    size_t get_arity() const { return arity; }
    const std::vector<ArgDomain>& get_domains() const { return domains; }

    ReturnType evaluate(const StateVector& state_vector) const {
        if (state_vector.size() != arity) {
            throw std::invalid_argument(
                "[Function Context] State vector size does not match the dimensionality"
            );
        }

        return std::apply(eval_func, std::make_tuple(state_vector));
    }

private:
    const EvalFunc eval_func;
    const size_t arity;
    const std::vector<ArgDomain>& domains;
};

#endif
