#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#include "csv_handler.h"
#include "utils/data.h"

using namespace AppTypes;


std::vector<double> CSVHandler::read_line(std::ifstream& input) {
    std::string line;
    if (!std::getline(input, line)) {
        throw std::runtime_error("[CSV parsing] Failed to read line.");
    }

    std::vector<double> values;
    std::istringstream iss(line);
    std::string token;

    while (std::getline(iss, token, ',')) {
        try {
            values.push_back(std::stod(token));
        } catch (const std::invalid_argument&) {
            throw std::runtime_error("[CSV parsing] Failed to parse numeric value.");
        }
    }

    return values;
}

Dataset CSVHandler::load_data(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("[CSV parsing] Failed to open input file.");
    }

    auto row_values = read_line(file);
    if (row_values.size() < 2) {
        throw std::runtime_error(
            "[CSV parsing] Invalid file format. Datapoint must contain at least one predictor."
        );
    }

    Dataset data;
    const size_t point_dim = row_values.size() - 1;
    data.emplace_back(make_data_point(row_values));

    while (file.peek() != EOF) {
        row_values = read_line(file);

        if (row_values.size() != point_dim + 1) {
            throw std::runtime_error(
                "[CSV parsing] Invalid file format. All datapoints must have the number of predictors."
            );
        }
        
        data.emplace_back(make_data_point(row_values));
    }

    file.close();
    return data;
}


void CSVHandler::write_line(std::ofstream& output, const std::vector<double>& values) {
    if (!output.is_open()) {
        throw std::runtime_error("[CSV parsing] Output file is not open.");
    }

    for (size_t i = 0; i < values.size(); ++i) {
        output << values[i];
        if (i < values.size() - 1) {
            output << ",";
        }
    }
    output << "\n";
}


void CSVHandler::save_predictions(
    const std::string& file_path,
    const std::vector<PredictorSet>& predictors,
    const std::vector<double>& predictions
){
    if (predictors.size() != predictions.size()) {
        throw std::invalid_argument(
            "[CSV parsing] Predictor and prediction vectors must have the same size."
        );
    }

    std::ofstream output(file_path, std::ios::out);
    if (!output.is_open()) {
        throw std::runtime_error("[CSV parsing] Failed to open output file.");
    }

    for (size_t i = 0; i < predictors.size(); ++i){
        std::vector<double> row_values = predictors[i];
        row_values.push_back(predictions[i]);
        write_line(output, row_values);
    }

    output.close();
}