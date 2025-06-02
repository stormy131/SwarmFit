#include <iostream>
#include <filesystem>

#include "csv_handler.h"
#include "fit_runner.h"
#include "evaluators/mse.h"
#include "models/linear.h"
#include "models/sine.h"
#include "const.h"

using namespace AppTypes;
using namespace Constants;
namespace fs = std::filesystem;

void print_usage() {
       std::cout << "Usage: program_name <model: linear|sine> <data_file> <log_file>" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 4) {
        std::cout << "[MAIN]: Invalid number of arguments" << std::endl;
        print_usage();
        return 1;
    }

    std::string model_type = argv[1];
    std::string input_file = argv[2];
    std::string output_file = argv[3];

    if (model_type != "linear" && model_type != "sine") {
        std::cout << "[MAIN]: Specified model type is not supported" << std::endl;
        print_usage();
        return 1;
    }

    if (!fs::exists(input_file)) {
        std::cout << "[MAIN]: Specified data file does not exist" << std::endl;
        return 1;
    }

    const Dataset data = CSVHandler::load_data(input_file);

    std::shared_ptr<BaseModel> model;
    if (model_type == "linear") {
        model = std::make_shared<LinearModel>(data[0].first.size());
    } else {
        model = std::make_shared<SineModel>(data[0].first.size());
    }

    const auto evaluator = std::make_shared<MSEEvaluator>(model);

    auto runner = FitRunner(model, evaluator, data);
    const ParamsArray fit_params = runner.fit(MIN_DELTA, N_TOLERANCE, MAX_STEPS);

    std::vector<PredictorSet> predictors(data.size());
    std::vector<double> predictions(data.size());

    std::transform(data.begin(), data.end(), predictors.begin(),
        [&](DataPoint dp) -> PredictorSet { return dp.first; }
    );
    std::transform(data.begin(), data.end(), predictions.begin(),
        [&](DataPoint dp) -> double {
            return model->predict(dp.first, fit_params);
        }
    );

    CSVHandler::save_predictions(output_file, predictors, predictions);

    return 0;
}
