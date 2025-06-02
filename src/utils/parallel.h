#ifndef PARALLEL_H
#define PARALLEL_H

#include <future>
#include <vector>
#include <functional>

template <typename T, size_t N_ARGS>
void compute_on_dims(
    const std::array<std::vector<T>, N_ARGS>& arg_arrays,
    std::vector<T>& result,
    const size_t start_dim,
    const size_t end_dim,
    const std::function<T(std::array<T, N_ARGS>&)> op
) {
    for (size_t i = start_dim; i < end_dim; ++i) {
        std::array<T, N_ARGS> call_args;
        for (size_t j = 0; j < N_ARGS; ++j) {
            call_args[j] = arg_arrays[j][i];
        }

        result[i] = op(call_args);
    }
}


template <typename T, size_t N_ARGS>
std::vector<T> parallelize(
    const std::array<std::vector<T>, N_ARGS>& args,
    const std::function<T(const std::array<T, N_ARGS>&)>& op,
    const size_t dim
) {
    for (auto&& arg : args) {
        if (arg.size() < dim) {
            throw std::invalid_argument(
                "[Parallelize] Argument dimensionality does not align with provided value"
            );
        }
    }

    const size_t num_tasks = std::min(
        static_cast<size_t>(std::thread::hardware_concurrency()),
        dim
    );

    // Even distribution of compute per task
    const size_t dims_per_task = dim / num_tasks;
    const size_t rem = dim % num_tasks;

    std::vector<T> result;
    result.resize(dim);

    std::vector<std::future<void>> futures;
    futures.resize(num_tasks);

    size_t end_idx = 0;
    for (size_t i = 0; i < num_tasks; ++i) {
        const size_t start_idx = end_idx;
        end_idx = start_idx + dims_per_task;
        end_idx = std::min(end_idx + (i < rem ? 1 : 0), dim);

        futures[i] = std::async(
            std::launch::async,
            [&]() {
                compute_on_dims<T, N_ARGS>(args, result, start_idx, end_idx, op);
            }
        );
    }

    for (auto& future : futures) {
        future.get();
    }

    return result;
}

#endif
