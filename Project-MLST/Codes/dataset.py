import networkx as nx
import random
import matplotlib.pyplot as plt
from tqdm import tqdm
import os
import argparse

# Function to ensure a random graph is connected
def generate_connected_random_graph(n, p):
    while True:
        graph = nx.erdos_renyi_graph(n, p)
        if nx.is_connected(graph):
            return graph

# Function to create a D-regular graph
def generate_d_regular_graph(n, k):
    if n * k % 2 != 0:
        raise ValueError("n * k must be even for a D-regular graph.")
    return nx.random_regular_graph(k, n)

# Function to create a complete grid graph
def generate_complete_grid_graph(rows, cols):

    return nx.convert_node_labels_to_integers(nx.grid_2d_graph(rows, cols))

# Function to create an incomplete grid graph
def generate_incomplete_grid_graph(rows, cols, k):
    grid_graph = nx.grid_2d_graph(rows, cols)
    # Convert to integers for easier edge manipulation
    grid_graph = nx.convert_node_labels_to_integers(grid_graph)
    edges = list(grid_graph.edges())
    while True:
        random.shuffle(edges)
        edges_to_remove = edges[:k]
        grid_graph.remove_edges_from(edges_to_remove)
        if nx.is_connected(grid_graph):
            break
        else:
            grid_graph.add_edges_from(edges_to_remove)
        
    return grid_graph

def write_graph_to_file(graph, filename):
    """
    Writes a graph to a file in the specified format:
    - First line: num_nodes num_edges
    - Subsequent lines: each edge as "node1 node2"

    Parameters:
        graph (networkx.Graph): The graph to write.
        filename (str): The name of the output file.
    """
    os.makedirs(os.path.dirname(filename), exist_ok=True)
    with open(filename, "w") as file:
        # Write the number of nodes and edges
        num_nodes = graph.number_of_nodes()
        num_edges = graph.number_of_edges()
        file.write(f"{num_nodes} {num_edges}\n")

        # Write each edge on a new line
        for edge in graph.edges():
            file.write(f"{edge[0]} {edge[1]}\n")

cnt=0
# Visualization function
def visualize_graph(graph, title):
    global cnt
    plt.figure(figsize=(8, 6))
    nx.draw(graph, with_labels=True, node_size=700, font_size=10)
    plt.title(title)
    plt.savefig(f"{cnt}.png")  # Save the figure
    plt.close()
    cnt+=1

def generate_random_dataset(nodes_list=None, probs_list=None, num_graphs=10):
    if nodes_list is None:
        nodes_list = [10, 20, 30, 40]
    if probs_list is None:
        probs_list = [0.1, 0.2, 0.3,0.4,0.5]

    for i in tqdm(range(num_graphs), desc="Generating random graphs"):
        for n in nodes_list:
            for p in probs_list:
                graph = generate_connected_random_graph(n, p)
                write_graph_to_file(graph, f"Dataset/Random/{n}_nodes_{p}_probability_{i}.txt")

def generate_d_regular_dataset(nodes_list=None, degree_list=None, num_graphs=10):
    if nodes_list is None:
        nodes_list = [10, 20, 30]
    if degree_list is None:
        degree_list = [3, 4, 5]

    for i in tqdm(range(num_graphs), desc="Generating D-regular graphs"):
        for n in nodes_list:
            for k in degree_list:
                graph = generate_d_regular_graph(n, k)
                write_graph_to_file(graph, f"Dataset/D-Regular/{n}_nodes_{k}_degree_{i}.txt")

def generate_complete_grid_dataset(rows_list=None, cols_list=None, num_graphs=10):
    if rows_list is None:
        rows_list = [4, 5, 6, 7 ,8 ,9]
    if cols_list is None:
        cols_list = [4, 5, 6 , 7 , 8, 9]

    for i in tqdm(range(num_graphs), desc="Generating complete grid graphs"):
        for rows in rows_list:
            for cols in cols_list:
                if cols < rows:
                    continue
                graph = generate_complete_grid_graph(rows, cols)
                write_graph_to_file(graph, f"Dataset/Complete-Grid/{rows}_rows_{cols}_cols_{i}.txt")

def generate_incomplete_grid_dataset(rows_list=None, cols_list=None, num_graphs=10):
    if rows_list is None:
        rows_list = [4, 5, 6, 7, 8, 9]
    if cols_list is None:
        cols_list = [4, 5, 6, 7, 8, 9]

    for i in tqdm(range(num_graphs), desc="Generating incomplete grid graphs"):
        for rows in rows_list:
            for cols in cols_list:
                if cols < rows:
                    continue
                
                k_list = [int(2*rows*cols/10), int(4*rows*cols/10), int(6*rows*cols/10)]
                for k in k_list:
                    graph = generate_incomplete_grid_graph(rows, cols, k)
                    write_graph_to_file(graph, f"Dataset/Incomplete-Grid/{rows}_rows_{cols}_cols_{k}_missing_edges_{i}.txt")
    
# Generate graphs
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate various types of graphs.")
    parser.add_argument("--num", type=int, nargs='?', default=10, help="Number of graphs to generate for each type (default: 10).")
    args = parser.parse_args()

    num_graphs = args.num

    # 1. Random graph with different nodes and probabilities (must be connected)
    generate_random_dataset(num_graphs=num_graphs)

    # 2. D-regular graph for n nodes and k degree
    generate_d_regular_dataset(num_graphs=num_graphs)

    # 3. Complete grid graph for rows and columns
    generate_complete_grid_dataset(num_graphs=num_graphs)

    # 4. Incomplete grid graph for rows, columns, and k edges removed
    generate_incomplete_grid_dataset(num_graphs=num_graphs)
