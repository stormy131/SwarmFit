#ifndef CSV_HANDLER_H
#define CSV_HANDLER_H

#include <string>
#include <vector>

#include "types.h"

class CSVHandler {
    static std::vector<double> read_line(std::ifstream& input);
    static void write_line(std::ofstream& output, const std::vector<double>& values);

public:
    // Data loading API
    [[nodiscard]] static AppTypes::Dataset load_data(const std::string& file_path);
    void static save_predictions(
        const std::string& file_path,
        const std::vector<AppTypes::PredictorSet>& predictors,
        const std::vector<double>& predictions
    );
};

#endif
