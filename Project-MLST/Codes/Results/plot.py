# import pandas as pd
# import matplotlib.pyplot as plt

# # Load the CSV file into a DataFrame
# file_path = "Random.csv"  # Replace with your CSV file path
# data = pd.read_csv(file_path)

# # Extract relevant columns
# algo_names = data.columns[2:]  # Assuming the first two columns are "Nodes" and "Degree"
# algo_names = [algo_names[i] for i in range(len(algo_names)) if i % 2 != 0]
# algo_names = algo_names[:-1]
# # Plot curves for each row in the DataFrame
# plt.figure(figsize=(12, 6))
# for index, row in data.iterrows():
#     leaf_counts = row[algo_names]

#     # Plot curve
#     plt.plot(algo_names, leaf_counts, marker='o', label=f"Row: {index}")

# # Customize plot
# plt.xlabel("Algorithms", fontsize=12)
# plt.ylabel("Leaf Count", fontsize=12)
# plt.title("Leaf Count vs. Algorithms for Each Row", fontsize=14)
# plt.xticks(rotation=45)
# plt.legend()
# plt.tight_layout()

# # Show plot
# plt.savefig('comparison_plot_random.png')
import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV file into a DataFrame
file_path = "Random.csv"  # Replace with your CSV file path
data = pd.read_csv(file_path)

# Extract relevant columns
algo_names = data.columns[2:]  # Assuming the first two columns are "Nodes" and "Degree"
algo_names = [algo_names[i] for i in range(len(algo_names)) if i % 2 != 0]
algo_names = algo_names[:-1]

# Transpose the data to have rows on the x-axis and algorithms as lines
transposed_data = data.set_index(['Nodes', 'Probability']).T

# Filter to keep only odd-indexed columns and remove the last column
transposed_data = transposed_data.loc[algo_names]

# Plot curves for each algorithm
plt.figure(figsize=(12, 6))
for algo in transposed_data.index:
    plt.plot(transposed_data.columns, transposed_data.loc[algo], marker='o', label=algo)

# Customize plot
plt.xlabel("Rows", fontsize=12)
plt.ylabel("Leaf Count", fontsize=12)
plt.title("Leaf Count vs. Rows for Each Algorithm", fontsize=14)
plt.xticks(rotation=45)
plt.legend()
plt.tight_layout()

# Save plot
plt.savefig('comparison_plot_random.png')

# Show plot
plt.show()