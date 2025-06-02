from argparse import ArgumentParser, Namespace

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd


parser = ArgumentParser(description="Generate synthetic data for regression task.")
parser.add_argument("--task", "-t", choices=["generate", "plot"], default="generate",
                    help="Task to perform: 'generate' to create data, 'plot' to visualize provided predictions")
parser.add_argument("--n_samples", default=100,
                    help="Number of samples to generate [for 'generate' task].")
parser.add_argument("--n_dims", default=3,
                    help="Number of dimensions for the input data [for 'generate' task].")
parser.add_argument("--scale", "-s", default=5,
                    help="Scale factor for the random input data [for 'generate' task].")
parser.add_argument("--actual", default=None,
                    help="Input CSV file, containing tested dataset [for 'plot' task].")
parser.add_argument("--predicted", default=None,
                    help="Input CSV file, containing predictions for tested dataset [for 'plot' task].")
parser.add_argument("--output", "-o", default=None, required=True,
                    help="Output file path for the generated data / plot. Provided filepath is required to have appropriate extension (e.g. .csv/.png respectively).")
args = parser.parse_args()


def target_function(X: np.ndarray) -> np.ndarray:
    return (X * 3).sum(axis=1) - 5


def generate_data(n_samples: int, n_dim: int, scale: float) -> pd.DataFrame:
    Xs = np.random.random((n_samples, n_dim)) * 50
    Ys = target_function(Xs)
    noise = np.random.normal(scale=scale, size=Xs.shape[0])

    data_df = pd.DataFrame(np.hstack((Xs, (Ys + noise).reshape(-1, 1))))
    return data_df


def plot_data(actual_path: str, predicted_path: str, output_path: str) -> None:
    actual = pd.read_csv(actual_path, header=None).to_numpy()
    predicted = pd.read_csv(predicted_path, header=None).to_numpy()
    Ys = actual[:, -1]

    plt.figure(figsize=(10, 6))
    plt.scatter(actual[:, 0], Ys.flatten(), label="Train data", alpha=0.5, color="blue")
    plt.plot(predicted[:, 0], predicted[:, 1], label="Test prediction", color="red")
    plt.grid()
    plt.xlabel("Input Feature")
    plt.ylabel("Target Value")
    plt.title("Actual vs Predicted Values")
    plt.legend()
    plt.savefig(output_path)


def main(args: Namespace) -> None:
    assert args.output, "Output file path is expectedd."

    match args.task:
        case "generate":
            data_df = generate_data(
                int(args.n_samples),
                int(args.n_dims),
                int(args.scale),
            )
            data_df.to_csv(args.output, sep=",", header=False, index=False)

        case "plot":
            assert None not in [args.actual, args.predicted], "Actual/predicted filepaths for plotting must be provided."
            plot_data(args.actual, args.predicted, args.output)

        case _:
            raise ValueError("Invalid task specified.")

if __name__ == "__main__":
    main(args)
 