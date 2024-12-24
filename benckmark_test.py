import time
import openpyxl
from availability_evaluation import *
import pandas as pd
import os
import subprocess

def binding_cpp_files():
    print("Binding the CPP files")
    if not os.path.exists("./build"):
        print("Creating the build directory")
        os.makedirs("./build")
    try:
        subprocess.run(["make", "clean"], cwd="./build", check=True)
        subprocess.run(["make"], cwd="./build", check=True)
    except subprocess.CalledProcessError as e:
        print(f"An error occurred during the make process: {e}")
        raise e
    finally:
        print("Binding the CPP files completed")


def run_test(topology_name):
    print("===============Running tests for " + topology_name + "===============")
    
    # Measure the time taken
    time_start = time.time()

    # Load the topology
    print("Loading topology: " + topology_name)
    G, _, _ = read_graph('topologies/' + topology_name, topology_name)

    # Load the 0.9, 0.99, 0.999 Availability from PYRBD       
    print("Loading 0.9, 0.99, 0.999 Availability results from PYRBD")
    avail_path = os.path.join('topologies', topology_name, topology_name + '_availability.xlsx')

    # Load the availability results and keep
    avail_df = pd.read_excel(avail_path, dtype={'source': int, 'target': int, 
                                                '0.9 Availability': float, 
                                                '0.99 Availability': float, 
                                                '0.999 Availability': float})
    
    # Set the precision
    precision = 9
    print(f"Setting the comparsion precision to {precision}")

    # Get all the node pairs
    node_pairs = list(combinations(list(G.nodes), 2))

    # Create the availability dictionaries
    A_dic_09 = {i:0.9 for i in range(G.number_of_nodes())}
    A_dic_099 = {i:0.99 for i in range(G.number_of_nodes())}
    A_dic_0999 = {i:0.999 for i in range(G.number_of_nodes())}

    # Prepare result list for CSV output
    result_data = []

    # Evaluate the availability with CPP
    print("Testing the evaluating of the availability with CPP")
    for src, dst in node_pairs:
        avail_09 = round(calculate_availability_cpp(G, src, dst, A_dic_09), precision)
        avail_099 = round(calculate_availability_cpp(G, src, dst, A_dic_099), precision)
        avail_0999 = round(calculate_availability_cpp(G, src, dst, A_dic_0999), precision)

        # Get the availability from the excel file
        excel_09 = round(avail_df.loc[(avail_df['source'] == src+1) & (avail_df['target'] == dst+1), '0.9 Availability'].values[0], precision)
        excel_099 = round(avail_df.loc[(avail_df['source'] == src+1) & (avail_df['target'] == dst+1), '0.99 Availability'].values[0], precision)
        excel_0999 = round(avail_df.loc[(avail_df['source'] == src+1) & (avail_df['target'] == dst+1), '0.999 Availability'].values[0], precision)

        if not np.isclose(avail_09, excel_09, atol=10**-precision):
            raise Exception(f"Error in 0.9 availability for {src} -> {dst}. CPP: {avail_09}, Excel: {excel_09}")
        if not np.isclose(avail_099, excel_099, atol=10**-precision):
            raise Exception(f"Error in 0.99 availability for {src} -> {dst}. CPP: {avail_099}, Excel: {excel_099}")
        if not np.isclose(avail_0999, excel_0999, atol=10**-precision):
            raise Exception(f"Error in 0.999 availability for {src} -> {dst}. CPP: {avail_0999}, Excel: {excel_0999}")

        result_data.append({
            'source': src + 1,
            'target': dst + 1,
            '0.9 Availability': avail_09,
            '0.99 Availability': avail_099,
            '0.999 Availability': avail_0999
        })

    print(topology_name + " tests passed")   

    time_end = time.time()
    time_taken = round(time_end - time_start, 2)
    print("Time taken: " + str(time_taken) + " seconds")

    # Save the results to a CSV file
    result_df = pd.DataFrame(result_data)
    result_df.to_csv(f"results/{topology_name}_availability_cpp.csv", index=False)

def run_benchmark(topology_name):
    print("===============Running benchmark for " + topology_name + "===============")

    # Load the topology
    print("Loading topology: " + topology_name)
    G, _, _ = read_graph('topologies/' + topology_name, topology_name)

    # Set the precision
    precision = 9
    print(f"Setting the comparsion precision to {precision}")
    
    # Get all the node pairs
    node_pairs = list(combinations(list(G.nodes), 2))

    # Create the availability dictionaries
    A_dic_09 = {i:0.9 for i in range(G.number_of_nodes())}

    result_data = []
    time_total_cpp = 0
    time_total_py = 0
    # Evaluate the availability with CPP
    for src, dst in node_pairs:
        # Measure the CPP time taken
        time_start_cpp = time.time()
        calculate_availability_cpp(G, src, dst, A_dic_09)
        time_end_cpp = time.time()

        # Measure the Python time taken
        time_start_py = time.time()
        calculate_availability(G, src, dst, A_dic_09)
        time_end_py = time.time()

        time_cpp = round(time_end_cpp - time_start_cpp, precision)
        time_py = round(time_end_py - time_start_py, precision)
        time_total_cpp += time_cpp
        time_total_py += time_py

        result_data.append({
            'source': src + 1,
            'target': dst + 1,
            'CPP Time': time_cpp,
            'Python Time': time_py
        })
    
    result_data.append({
        'source': 'All',
        'target': 'All',
        'CPP Time': time_total_cpp,
        'Python Time': time_total_py
    })

    # Save the results to a CSV file
    result_df = pd.DataFrame(result_data)
    result_df.to_csv(f"results/{topology_name}_benchmark.csv", index=False)

    print(f"{topology_name} benchmark completed:")    
    print(f"Total time taken for CPP: {time_total_cpp} seconds")
    print(f"Total time taken for Python: {time_total_py} seconds")
    print(f"Total time saved by CPP: {round(time_total_py - time_total_cpp, precision)} seconds")

if __name__ == "__main__":
    # binding_cpp_files()
    run_test('Abilene')
    run_test('Germany_17')
    run_test('HiberniaUk')
    run_test('polska')
    run_test('dfn-bwin')
    run_benchmark('Abilene')
    run_benchmark('Germany_17')
    run_benchmark('HiberniaUk')
    run_benchmark('polska')
    run_benchmark('dfn-bwin')
    print("All tests and benchmarks completed")