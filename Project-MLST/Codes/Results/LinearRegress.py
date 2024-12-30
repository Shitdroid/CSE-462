import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression

# Load the data from a CSV file
# Assume the CSV file has columns named 'x' and 'y'
file_path = "Incomplete-Grid.csv"  # Replace with your file path
data = pd.read_csv(file_path)

# Extract x and y values
x = data['Edges'].values.reshape(-1, 1)
y = data['Approx Algorithm Time'].values

# grouped_data = data.groupby('Nodes')['Exact Algorithm Time'].max().reset_index()
#
# grouped_data = grouped_data[pd.to_numeric(grouped_data['Exact Algorithm Time'], errors='coerce').notnull()]
#
#
# # Extract x and y values
# x = grouped_data['Nodes'].values.reshape(-1, 1)
# y = grouped_data['Exact Algorithm Time'].astype(float).values

print(y)
model = LinearRegression()
model.fit(x, y)

# Predict y values using the regression line
y_pred = model.predict(x)

# Plot the data points
plt.scatter(x, y, color='blue', label='Data Points')

# Plot the regression line
plt.plot(x, y_pred, color='red', label='Regression Line')

# Highlight the deviations
for xi, yi, ypi in zip(x, y, y_pred):
    plt.plot([xi, xi], [yi, ypi], color='gray', linestyle='--', linewidth=0.7)

# Add labels, title, and legend
plt.xlabel('Number of Edges')
plt.ylabel('Time Taken')
plt.title('Incomplete Grid Graph')
plt.legend()
plt.grid(True)

# Show the plot
plt.savefig('approx_time_plot_incomplete_grid.png')
