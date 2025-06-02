cmake_minimum_required(VERSION 3.30)
project(SwarmFit)

set(CMAKE_CXX_STANDARD 20)

add_executable(SwarmFit main.cpp
        pso/optimizer.h
        pso/particle.h
        pso/context.h
        pso/pso_config.h
        utils/parallel.h
        csv_handler.cpp
        csv_handler.h
        types.h
        utils/data.h
        models/base.h
        models/linear.h
        evaluators/base.h
        evaluators/mse.h
        models/sine.h
        fit_runner.h
        utils/random.h
        models/linear.cpp
        models/sine.cpp
        evaluators/mse.cpp
        fit_runner.cpp
        config.h
        const.h
)
