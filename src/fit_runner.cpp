#include "fit_runner.h"

#include "config.h"
#include "const.h"
#include "utils/data.h"

Optimizer<FitRunner::TargetContext> FitRunner::setup_optimizer(int max_steps) {
    const auto& domains = _model->get_param_domains();
    const auto context = TargetContext(
        eval_fit,
        domains.size(),
        domains
    );
    const OptimizerConfig run_config {
        .rnd_seed = Constants::SEED,
        .n_steps = max_steps,
        .n_particles = Constants::N_PARTICLES,

        .swarm_config ={
            .w_inertia = Constants::W_INERTIA,
            .cognitive_coef = Constants::COGNITIVE_COEF,
            .social_coef = Constants::SOCIAL_COEF,
        }
    };

    return {std::move(context), run_config};
}

FitRunner::ParamsArray FitRunner::fit(double min_delta, int n_tolerance, int max_steps) {
    if (!_evaluator || !_model) {
        throw std::runtime_error("[FitRunner] Evaluator or model is not initialized.");
    }

    auto pso = setup_optimizer(max_steps);
    ParamsArray current_params;

    size_t stagnation_counter{};
    double current_loss{};
    double best_loss = std::numeric_limits<double>::max();
    for (size_t i = 0; i < max_steps; ++i) {
        if (stagnation_counter >= n_tolerance) {
            std::cout << "Optimization suspended at step " << i << "\n";
            break;
        }

        current_params = pso.step();
        current_loss = (-1) * eval_fit(current_params);

        if (best_loss - current_loss > min_delta) {
            best_loss = current_loss;
            stagnation_counter = 0;
        } else {
            ++stagnation_counter;
        }
    }

    std::cout << "Final error: " << current_loss << "\n";
    std::cout << "Acquired parameters: ";
    std::cout << vector_to_string(current_params) << "\n";

    return current_params;
}