#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <valarray>
#include <functional>

#include "pso_config.h"
#include "../config.h"
#include "../utils/data.h"
#include "../utils/parallel.h"
#include "../utils/random.h"


template <typename Context>
class Particle {
    using ArgType          = typename Context::ArgType;
    using ArgDomain        = typename Context::ArgDomain;
    using ArgDistribution  = typename Context::ArgDistribution;
    using StateVector      = typename Context::StateVector;

    ParticleConfig<Context>& config {};
    const Context& function_context {};

    StateVector velocity, position, found_best;
    std::mt19937 gen;

    /**
     * Updates particle's velocity, using swarm configuration and global/local best found position.
     * Vector operation is parallelized across its dimensions.
     */
    void update_velocity() {
        if (position.size() != function_context.get_arity()) {
            throw std::invalid_argument(
                "[Particle] Particle's state dimensions don't align with target function"
            );
        }

        // "args" parameter contents: [old_velocity, position,  found_best, global_best]
        auto update_rule = [this](const std::array<ArgType, 4>& args) -> ArgType {
            auto swarm_config = config.get_swarm_config();
            return (
                args[0] * swarm_config.w_inertia +
                random_value(0.0, 1.0, gen) * swarm_config.cognitive_coef * (args[2] - args[1]) +
                random_value(0.0, 1.0, gen) * swarm_config.social_coef * (args[3] - args[1])
            );
        };

        std::array<StateVector, 4> tmp = {velocity, position, found_best, config.get_global_best()};
        auto result = parallelize(
            tmp,
            std::function(update_rule),
            function_context.get_arity()
        );

        std::copy(std::begin(result), std::end(result),  velocity.begin());
    }

public:
    /**
     * Particle constructor. Includes velocity initialization and static assertion of initial position's shape
     * (must correspond to the targeted function's arity).
     *
     * @param init_position - particle's initial position in the search space.
     * @param configuration - particle's configuration with swarm settings and optimization consts.
     */
    Particle(
        const StateVector& init_position,
        ParticleConfig<Context>& configuration
    ) :
        config(configuration),
        function_context(configuration.get_func_context()),
        position(init_position),
        found_best(init_position),
        velocity(function_context.get_arity(), ArgType{}),
        gen( std::mt19937{std::random_device{}()} )
    {

        // Initiate particle's velocity vector. Samples each dimension from uniform distribution with bounds
        // ( -|LOW - HIGH|, |LOW - HIGH| ).
        std::transform(
            function_context.get_domains().cbegin(),
            function_context.get_domains().cend(),
            velocity.begin(),
            [this](ArgDomain domain) -> ArgType {
                return random_value(
                    domain.first - domain.second,   // Low
                    domain.second - domain.first,   // High
                    gen
                );
            }
        );
    }


    /**
     * Performs particle's step in the optimization procedure, applying position change with velocity vector.
     * Best found local/global positions and velocity will be updated.
     */
    void step() {
        update_velocity();

        std::transform(
            position.cbegin(), position.cend(),
            velocity.cbegin(), position.begin(),
            [](ArgType p_i, ArgType v_i) -> ArgType { return p_i + v_i; }
        );

        // Mirroring "out of domain"
        std::transform(
            position.cbegin(), position.cend(),
            function_context.get_domains().cbegin(), position.begin(),
            [](ArgType p_i, ArgDomain domain) -> ArgType {
                if (p_i < domain.first)
                    return 2 * domain.first - p_i;

                if (p_i > domain.second)
                    return 2 * domain.second - p_i;

                return p_i;
            }
        );

        auto current_e = function_context.evaluate(position);
        auto local_e = function_context.evaluate(found_best);
        auto global_e = function_context.evaluate(config.get_global_best());

        DEBUG_PRINT(vector_to_string(position));
        DEBUG_PRINT(
            "[" + std::to_string(current_e) + " " +
                  std::to_string(local_e)   + " " +
                  std::to_string(global_e)  +
            "]"
        );

        if(current_e > local_e) {
            found_best = position;

            if(current_e > global_e){
                DEBUG_PRINT("***");
                config.set_global_best(position);
            }
        }
    }
};

#endif