import networkx as nx
import random
import matplotlib.pyplot as plt

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
    random.shuffle(edges)
    edges_to_remove = edges[:k]
    grid_graph.remove_edges_from(edges_to_remove)
    write_graph_to_file(grid_graph,f"grid_graph_{rows}row_{cols}col_{k}missing")
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

# Generate graphs
if __name__ == "__main__":
    # 1. Random graph with different nodes and probabilities (must be connected)
    n = 10  # Number of nodes
    p = 0.3  # Edge probability
    random_graph = generate_connected_random_graph(n, p)
    visualize_graph(random_graph, "Connected Random Graph")

    # 2. D-regular graph for n nodes and k degree
    n = 10  # Number of nodes
    k = 3   # Degree
    d_regular_graph = generate_d_regular_graph(n, k)
    visualize_graph(d_regular_graph, f"{k}-Regular Graph")

    # 3. Complete grid graph for rows and columns
    rows = 4
    cols = 5
    complete_grid_graph = generate_complete_grid_graph(rows, cols)
    visualize_graph(complete_grid_graph, f"Complete Grid Graph ({rows}x{cols})")

    # 4. Incomplete grid graph for rows, columns, and k edges removed
    rows = 4
    cols = 5
    k = 5  # Number of edges to remove
    incomplete_grid_graph = generate_incomplete_grid_graph(rows, cols, k)
    visualize_graph(incomplete_grid_graph, f"Incomplete Grid Graph ({rows}x{cols})")
