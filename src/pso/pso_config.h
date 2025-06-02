#ifndef CONFIG_HPP
#define CONFIG_HPP

// Swarm optimization constants
struct SwarmConfig {
    const float w_inertia { 0.1 };
    const float cognitive_coef { 0.5 };
    const float social_coef { 0.5 };
};


// Optimization process configuration
struct OptimizerConfig {
    const unsigned long rnd_seed {};
    const int n_steps {};
    const int n_particles {};
    const SwarmConfig swarm_config {};
};


// Particle configuration
template<typename Context>
class ParticleConfig {
    using ArgType     = typename Context::ArgType;
    using StateVector = typename Context::StateVector;

    const SwarmConfig& swarm_config {};
    const Context& function_context;
    StateVector global_best {};

public:
    const SwarmConfig& get_swarm_config() const { return swarm_config; }
    const Context& get_func_context() const { return function_context; }

    const StateVector& get_global_best() const { return global_best; }
    void set_global_best(StateVector& position){ global_best = position; }

    ParticleConfig(
        const Context& context,
        StateVector&& global_best,
        const OptimizerConfig& outer_conf
    ) :
        swarm_config(outer_conf.swarm_config),
        function_context(context),
        global_best(global_best)
    {}
};

#endif