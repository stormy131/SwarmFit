#ifndef CONST_H
#define CONST_H

namespace Constants {
    // Fit configuration
    inline constexpr double MIN_DELTA   = 100;
    inline constexpr int N_TOLERANCE    = 10;

    // Optimizer configuration
    inline constexpr int SEED           = 13;
    inline constexpr int MAX_STEPS      = 100;
    inline constexpr int N_PARTICLES    = 100;

    // PSO swarm configuration
    inline constexpr double W_INERTIA       = 0.4;
    inline constexpr double COGNITIVE_COEF  = 0.3;
    inline constexpr double SOCIAL_COEF     = 0.8;
}

#endif
