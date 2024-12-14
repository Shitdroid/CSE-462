import subprocess
import os
import tqdm
import time
import csv

# ...existing code...

def compile_cpp_file(algo):
    """
    Compiles a C++ file and returns the path of the object file created.
    
    Args:
        source_file (str): The path to the C++ source file.
    
    Returns:
        str: The path to the created object file.
    """
    compile_command = ['g++', algo['source_file'], '-o', algo['object_file']]
    
    try:
        subprocess.run(compile_command, check=True)
    except subprocess.CalledProcessError as e:
        print(f"Compilation failed: {e}")
        return None
    
    return algo['object_file']

def run_object_file(object_file, input_file):
    with open(input_file) as file:
        start_time=time.time()
        result = subprocess.run([object_file], stdin=file, capture_output=True, text=True)
        end_time=time.time()
    return result.stdout, end_time-start_time

def run_on_random_dataset(algorithms, nodes_list=None, probs_list=None, num_graphs=10):
    if nodes_list is None:
        nodes_list = [10, 20, 30, 40]
    if probs_list is None:
        probs_list = [0.1, 0.2, 0.3,0.4,0.5]

    results = []
    header=["Nodes","Probability","Exact Algorithm Leaves", "Exact Algorithm Time", "Approx Algorithm Leaves", "Approx Algorithm Time", "BFS Leaves", "BFS Time", "Priority BFS Leaves", "Priority BFS Time", "Max Priority BFS Leaves", "Max Priority BFS Time", "Simulated Annealing Leaves", "Simulated Annealing Time"]
    for nodes in tqdm.tqdm(nodes_list, desc="Nodes"):
        for prob in tqdm.tqdm(probs_list, desc="Probabilities", leave=False):
            result=[]
            result.append(nodes)
            result.append(prob)
            for algo in algorithms:
                total_leaves=0
                total_time=0
                if algo['name'] == "Exact Algorithm" and nodes>36:
                    result.append('-')
                    result.append('-')
                    continue
                for i in tqdm.tqdm(range(num_graphs), desc=f"{algo['name']} Graphs", leave=False):
                    input_file=f"Dataset/Random/{nodes}_nodes_{prob}_probability_{i}.txt"
                    leave, time=run_object_file(algo["object_file"], input_file)
                    total_leaves+=int(leave)
                    total_time+=time
                result.append(total_leaves/num_graphs)
                result.append(total_time/num_graphs)
            results.append(result)
    filename="Results/Random.csv"
    os.makedirs(os.path.dirname(filename), exist_ok=True)        
    with open(filename, "w") as file:
        writer=csv.writer(file)
        writer.writerow(header)
        writer.writerows(results)

def run_on_d_regular_dataset(algorithms, nodes_list=None, degree_list=None, num_graphs=10):
    if nodes_list is None:
        nodes_list = [10, 20, 30]
    if degree_list is None:
        degree_list = [3, 4, 5]
    
    results = []
    header=["Nodes","Degree","Exact Algorithm Leaves", "Exact Algorithm Time", "Approx Algorithm Leaves", "Approx Algorithm Time", "BFS Leaves", "BFS Time", "Priority BFS Leaves", "Priority BFS Time", "Max Priority BFS Leaves", "Max Priority BFS Time", "Simulated Annealing Leaves", "Simulated Annealing Time"]
    for nodes in tqdm.tqdm(nodes_list, desc="Nodes"):
        for degree in tqdm.tqdm(degree_list, desc="Degrees", leave=False):
            result=[]
            result.append(nodes)
            result.append(degree)
            for algo in algorithms:
                total_leaves=0
                total_time=0
                if algo['name'] == "Exact Algorithm" and nodes>36:
                    result.append('-')
                    result.append('-')
                    continue
                for i in tqdm.tqdm(range(num_graphs), desc=f"{algo['name']} Graphs", leave=False):
                    input_file=f"Dataset/D-Regular/{nodes}_nodes_{degree}_degree_{i}.txt"
                    leave, time=run_object_file(algo["object_file"], input_file)
                    total_leaves+=int(leave)
                    total_time+=time
                result.append(total_leaves/num_graphs)
                result.append(total_time/num_graphs)
            results.append(result)
    filename="Results/D-Regular.csv"
    os.makedirs(os.path.dirname(filename), exist_ok=True)        
    with open(filename, "w") as file:  
        writer=csv.writer(file)
        writer.writerow(header)
        writer.writerows(results)

def run_on_complete_grid_dataset(algorithms, rows_list=None, cols_list=None, num_graphs=10):
    if rows_list is None:
        rows_list = [4, 5, 6, 7 ,8 ,9]
    if cols_list is None:
        cols_list = [4, 5, 6 , 7 , 8, 9]
    
    results = []
    header=["Rows","Cols","Exact Algorithm Leaves", "Exact Algorithm Time", "Approx Algorithm Leaves", "Approx Algorithm Time", "BFS Leaves", "BFS Time", "Priority BFS Leaves", "Priority BFS Time", "Max Priority BFS Leaves", "Max Priority BFS Time", "Simulated Annealing Leaves", "Simulated Annealing Time"]
    for rows in tqdm.tqdm(rows_list, desc="Rows"):
        for cols in tqdm.tqdm(cols_list, desc="Cols", leave=False):
            result=[]
            result.append(rows)
            result.append(cols)
            for algo in algorithms:
                total_leaves=0
                total_time=0
                if cols < rows:
                    continue
                if algo['name'] == "Exact Algorithm" and rows*cols>36:
                    result.append('-')
                    result.append('-')
                    continue
                for i in tqdm.tqdm(range(num_graphs), desc=f"{algo['name']} Graphs", leave=False):
                    input_file=f"Dataset/Complete-Grid/{rows}_rows_{cols}_cols_{i}.txt"
                    leave, time=run_object_file(algo["object_file"], input_file)
                    total_leaves+=int(leave)
                    total_time+=time
                result.append(total_leaves/num_graphs)
                result.append(total_time/num_graphs)
            results.append(result)
    filename="Results/Complete-Grid.csv"
    os.makedirs(os.path.dirname(filename), exist_ok=True)        
    with open(filename, "w") as file:
        writer=csv.writer(file)
        writer.writerow(header)
        writer.writerows(results)

def run_on_incomplete_grid_dataset(algorithms, rows_list=None, cols_list=None, num_graphs=10):
    if rows_list is None:
        rows_list = [4, 5, 6, 7 ,8 ,9]
    if cols_list is None:
        cols_list = [4, 5, 6 , 7 , 8, 9]
    
    results = []
    header=["Rows","Cols","Missing Edges","Exact Algorithm Leaves", "Exact Algorithm Time", "Approx Algorithm Leaves", "Approx Algorithm Time", "BFS Leaves", "BFS Time", "Priority BFS Leaves", "Priority BFS Time", "Max Priority BFS Leaves", "Max Priority BFS Time", "Simulated Annealing Leaves", "Simulated Annealing Time"]
    for rows in tqdm.tqdm(rows_list, desc="Rows"):
        for cols in tqdm.tqdm(cols_list, desc="Cols", leave=False):
            k_list = [int(2*rows*cols/10), int(4*rows*cols/10), int(6*rows*cols/10)]
            for k in tqdm.tqdm(k_list, desc="Missing Edges", leave=False):
                result=[]
                result.append(rows)
                result.append(cols)
                result.append(k)
                for algo in algorithms:
                    total_leaves=0
                    total_time=0
                    if cols < rows:
                        continue
                    if algo['name'] == "Exact Algorithm" and rows*cols>36:
                        result.append('-')
                        result.append('-')
                        continue
                    for i in tqdm.tqdm(range(num_graphs), desc=f"{algo['name']} Graphs", leave=False):
                        input_file=f"Dataset/Incomplete-Grid/{rows}_rows_{cols}_cols_{k}_missing_edges_{i}.txt"
                        leave, time=run_object_file(algo["object_file"], input_file)
                        total_leaves+=int(leave)
                        total_time+=time
                    result.append(total_leaves/num_graphs)
                    result.append(total_time/num_graphs)
                results.append(result)
    filename="Results/Incomplete-Grid.csv"
    os.makedirs(os.path.dirname(filename), exist_ok=True)        
    with open(filename, "w") as file:
        writer=csv.writer(file)
        writer.writerow(header)
        writer.writerows(results)
        
# ...existing code...

if __name__ == "__main__":
    algorithms=[
        {
            "name": "Exact Algorithm",
            "source_file": "Exact_Exponential_Algorithm.cpp",
            "object_file": "./Exact_Exponential_Algorithm.o",
        },
        {
            "name": "Approx Algorithm",
            "source_file": "approx_algorithm.cpp",
            "object_file": "./approx_algorithm.o",
        },
        {
            "name": "BFS",
            "source_file": "bfs.cpp",
            "object_file": "./bfs.o",
        },
        {
            "name": "Priority BFS",
            "source_file": "priority_bfs.cpp",
            "object_file": "./priority_bfs.o",
        },
        {
            "name": "Max Priority BFS",
            "source_file": "max_priority_bfs.cpp",
            "object_file": "./max_priority_bfs.o",
        },
        {
            "name": "Simulated Annealing",
            "source_file": "simulated_annealing.cpp",
            "object_file": "./simulated_annealing.o",
        }
    ]
    for algorithm in algorithms:
        object_file = compile_cpp_file(algorithm)
        if object_file is not None:
            print(f"Compiled {algorithm['name']} successfully.")
        else:
            print(f"Failed to compile {algorithm['name']}.")
    
    # run_on_random_dataset(algorithms)
    run_on_d_regular_dataset(algorithms)
    run_on_complete_grid_dataset(algorithms)
    run_on_incomplete_grid_dataset(algorithms)

