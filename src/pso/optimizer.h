#ifndef RUNNER_HPP
#define RUNNER_HPP

#include <iostream>
#include <algorithm>
#include <vector>
#include <random>

#include "pso_config.h"
#include "particle.h"
#include "context.h"
#include "../config.h"
#include "../utils/data.h"
#include "../utils/random.h"


template <typename Context>
class Optimizer {
    using ArgType                   = typename Context::ArgType;
    using ArgDomain                 = typename Context::ArgDomain;

    using StateVector               = typename Context::StateVector;
    using StateParticle             = Particle<Context>;

    int step_counter = 0;
    const Context function_context;
    const OptimizerConfig config {};

    ParticleConfig<Context> particle_config {};
    std::vector<StateParticle> particles;
    std::mt19937 gen;


    /**
     * Creates a particle in the provided search space. Particle's position is sampled from provided
     * distribution set.
     *
     * @param distributions - sampling distributions [per state's dimension] for each eval_func argument
     * @return StateParticle instance, representing created particle
     */
    StateParticle generate_particle(){//std::array<ArgDistribution, Arity>& distributions) {
        StateVector init_state {};
        init_state.resize(function_context.get_arity());

        std::transform(
            function_context.get_domains().cbegin(),
            function_context.get_domains().cend(),
            init_state.begin(),
            [this](ArgDomain d) -> ArgType {
                return random_value(d.first, d.second, gen);
            }
        );

        // "Global best" update, in case if better position was found
        auto best_found = function_context.evaluate(particle_config.get_global_best());
        if(function_context.evaluate(init_state) > best_found){
            // *particle_config.global_best = init_state;
            particle_config.set_global_best(init_state);
        }

        return { init_state, particle_config };
    }


    /**
     * Generate a swarm of N particles in the search space. A set of distributions are created, to sample particles'
     * positions uniformly in the optimization domain.
     */
    void init_search_space() {
        particles.reserve(config.n_particles);
        std::ranges::generate_n(
            std::back_inserter(particles), config.n_particles,
            [this]() -> StateParticle { return generate_particle(); }
        );
    }

public:
    /**
     * Optimizer constructor. Includes static assertion of floating point type on arguments' type
     *
     * @param context
     * @param configuration - optimization configuration including "swarm settings", considered domain and
     * optimization consts.
     */
    Optimizer(const Context&& context, const OptimizerConfig& configuration) :
        function_context(std::move(context)),
        config(configuration),
        particle_config(
            function_context,
            // std::make_shared<StateVector>(context.get_arity(), ArgType{}),
            std::move(StateVector(context.get_arity(), ArgType{})),
            config
        ),
        gen( std::mt19937{configuration.rnd_seed} )
    {
        static_assert(std::is_floating_point_v<ArgType>);
        init_search_space();
        DEBUG_PRINT(
            "Initial best: ------------> " +
            vector_to_string(particle_config.get_global_best())
        );
    }

    /**
     * Performs a single optimization step for all particles. Advances the state of each particle,
     * increments the step counter,  and returns the current best-known solution (`global_best`).
     *
     * @return StateVector The global best solution after the step.
     */
    StateVector step() {
        if (step_counter < config.n_steps) {
            for(auto&& particle : particles) {
                particle.step();
            }

            DEBUG_PRINT(
                "------------> " +
                vector_to_string(particle_config.get_global_best())
            );
            ++step_counter;
        }

        return particle_config.get_global_best();
    }


    /**
     * Starts sequential PSO optimization procedure. During config.N_STEPS each particle will make a step
     * in the search space according to its velocity and swarm settings.
     *
     * @return a RunReport instance, describing the results of optimization procedure
     */
    StateVector sequential_run() {
        while (step_counter < config.n_steps) step();

        std::cout << "Best found: ";
        print_vector(particle_config.get_global_best());
        return particle_config.get_global_best();
    }
};


#endif
