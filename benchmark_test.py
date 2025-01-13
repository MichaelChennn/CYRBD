import time
from availability_evaluation import *
import pandas as pd
import os
import subprocess
import build.rbd_bindings
import ast
import csv



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


def run_test():
    # topologies = [folder for folder in os.listdir('topologies') if
    #                          os.path.isdir(os.path.join('topologies', folder))]
    
    topologies = ["Abilene", "dfn-bwin", "Germany_17", "HiberniaUk", "polska"]

    
    for topology in topologies:
        print("===============Running tests for " + topology + "===============")
        
        # Measure the time taken
        time_start = time.time()

        # Load the topology
        print("Loading topology: " + topology)
        G, _, _ = read_graph('topologies/' + topology, topology)

        # Load the 0.9, 0.99, 0.999 Availability from PYRBD       
        print("Loading 0.9, 0.99, 0.999 Availability results from PYRBD")
        avail_path = os.path.join('topologies', topology, topology + '_availability.xlsx')

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
            res_src = src + 1
            res_dst = dst + 1
            avail_09 = round(calculate_availability_cpp(G, src, dst, A_dic_09)[2], precision)
            avail_099 = round(calculate_availability_cpp(G, src, dst, A_dic_099)[2], precision)
            avail_0999 = round(calculate_availability_cpp(G, src, dst, A_dic_0999)[2], precision)

            # Get the availability from the excel file
            excel_09 = round(avail_df.loc[(avail_df['source'] == res_src) & (avail_df['target'] == res_dst), '0.9 Availability'].values[0], precision)
            excel_099 = round(avail_df.loc[(avail_df['source'] == res_src) & (avail_df['target'] == res_dst), '0.99 Availability'].values[0], precision)
            excel_0999 = round(avail_df.loc[(avail_df['source'] == res_src) & (avail_df['target'] == res_dst), '0.999 Availability'].values[0], precision)

            if not np.isclose(avail_09, excel_09, atol=10**-precision):
                raise Exception(f"Error in 0.9 availability for {src} -> {dst}. CPP: {avail_09}, Excel: {excel_09}")
            if not np.isclose(avail_099, excel_099, atol=10**-precision):
                raise Exception(f"Error in 0.99 availability for {src} -> {dst}. CPP: {avail_099}, Excel: {excel_099}")
            if not np.isclose(avail_0999, excel_0999, atol=10**-precision):
                raise Exception(f"Error in 0.999 availability for {src} -> {dst}. CPP: {avail_0999}, Excel: {excel_0999}")

            result_data.append({
                'source': res_src,
                'target': res_dst,
                '0.9 Availability': avail_09,
                '0.99 Availability': avail_099,
                '0.999 Availability': avail_0999
            })

        print(topology + " tests passed")   

        time_end = time.time()
        time_taken = round(time_end - time_start, precision)
        print("Time taken: " + str(time_taken) + " seconds")

        # Save the results to a CSV file
        result_df = pd.DataFrame(result_data)
        result_df.to_csv(f"results/{topology}_availability_cpp.csv", index=False)

    
def run_benchmark():
    # save the simulation times for each topology
    simulation_times_per_topology = {}

    # topologies = [folder for folder in os.listdir('topologies') if
    #                          os.path.isdir(os.path.join('topologies', folder))]
    
    topologies = ["Abilene", "dfn-bwin", "Germany_17", "HiberniaUk", "polska"]
    
    for topology in topologies:
        print("===============Running benchmark for " + topology + "===============")

        # Load the topology
        print("Loading topology: " + topology)
        G, _, _ = read_graph('topologies/' + topology, topology)
        
        # Create the availability dictionaries
        A_dic = {i:0.9 for i in range(G.number_of_nodes())}
        
        # Relabel the nodes of G and A_dic
        G, A_dic, relabel_mapping = relabel_graph_A_dict(G, A_dic)

        # Set the precision
        precision = 9
        print(f"Setting the comparsion precision to {precision}")
        
        # Get all the node pairs
        node_pairs = list(combinations(list(G.nodes), 2))

        result_data = []
        time_total_cpp = 0
        time_total_py = 0
        num_bool_expr_total = 0
        
        # Evaluate the availability with CPP
        for src, dst in node_pairs:
            
            # Measure the CPP time taken
            time_start_cpp = time.time()
            mincutsets = minimalcuts(G, src, dst)
            build.rbd_bindings.evaluateAvailability(mincutsets, A_dic, src, dst)
            time_end_cpp = time.time()
            
            # Measure the Python time taken
            time_start_py = time.time()
            process_topology(G, src, dst, A_dic)
            time_end_py = time.time()

            # Round the time taken
            time_cpp = round(time_end_cpp - time_start_cpp, precision)
            time_py = round(time_end_py - time_start_py, precision)

            # Get the number of boolean expressions
            num_bool_expr = build.rbd_bindings.lengthOfProbaset(mincutsets, src, dst)
            
            result_data.append({
                'source': src + 1,
                'target': dst + 1,
                'CPP Time': time_cpp,
                'Python Time': time_py,
                'Number of Boolean Expressions': num_bool_expr
            })

            time_total_cpp += time_cpp
            time_total_py += time_py
            num_bool_expr_total += num_bool_expr
            
        # Round the total time taken
        time_total_cpp = round(time_total_cpp, precision)
        time_total_py = round(time_total_py, precision)
    
        result_data.append({
            'source': 'All',
            'target': 'All',
            'CPP Time': time_total_cpp,
            'Python Time': time_total_py,
            'Number of Boolean Expressions': num_bool_expr_total
        })

        # Save the results to a CSV file for each topology
        result_df = pd.DataFrame(result_data)
        result_df.to_csv(f"results/{topology}_benchmark.csv", float_format='%.9f', index=False)

        # Format the time taken
        formatted_time_cpp = f"{time_total_cpp:.{precision}f}"[:10]
        formatted_time_py = f"{time_total_py:.{precision}f}"[:10]

        # Save the simulation times for a overall comparison
        simulation_times_per_topology[topology] = (formatted_time_cpp, formatted_time_py, num_bool_expr_total)

        print(f"{topology} benchmark completed:")    
        print(f"Total time taken for CPP: {time_total_cpp} seconds")
        print(f"Total time taken for Python: {time_total_py} seconds")
        print(f"Total time saved by CPP: {round(time_total_py - time_total_cpp, precision)} seconds")
    
    # Save the simulation times for each topology to a CSV file
    simulation_times_df = pd.DataFrame.from_dict(simulation_times_per_topology, orient='index', columns=['Simulation Time CPP (Second)', 'Simulation Time Python (Second)', 'Number of Boolean Expressions'])
    simulation_times_df.index.name = 'Topology'

    # Save the DataFrame to a CSV file
    simulation_times_df.to_csv("results/summary.csv", index=True)


def run_benchmark_multiprocessing():
    # save the simulation times for each topology
    simulation_times_per_topology = {}

    # topologies = [folder for folder in os.listdir('topologies') if
    #                          os.path.isdir(os.path.join('topologies', folder))]
    
    topologies = ["Abilene", "dfn-bwin", "Germany_17", "HiberniaUk", "polska"]

    for topology in topologies:
        print("===============Running benchmark multiprocessing for " + topology + "===============")

        # Load the topology
        print("Loading topology: " + topology)
        G, _, _ = read_graph('topologies/' + topology, topology)
        
        # Set the precision
        precision = 9
        print(f"Setting the comparsion precision to {precision}")
        
        # Create the availability dictionaries
        A_dic = {i:0.9 for i in range(G.number_of_nodes())}

        # Get the number of CPUs
        num_cpus = multiprocessing.cpu_count()
        print(f"Number of CPUs: {num_cpus}")

        # Evaluate the availability with CPP
        time_start_cpp = time.time()
        calculate_availability_multiprocessing_cpp(G, A_dic)
        time_end_cpp = time.time()
        # Evaluate the availability with Python 
        time_start_py = time.time()
        calculate_availability_multiprocessing(G, A_dic)
        time_end_py = time.time()

        time_cpp = round(time_end_cpp - time_start_cpp, precision)
        time_py = round(time_end_py - time_start_py, precision)

        # Format the time taken
        formatted_time_cpp = f"{time_cpp:.{precision}f}"[:10]
        formatted_time_py = f"{time_py:.{precision}f}"[:10]

        # Save the simulation times for a overall comparison
        simulation_times_per_topology[topology] = (formatted_time_cpp, formatted_time_py)

        print(f"{topology} benchmark completed:")
        print(f"Total time taken for CPP: {time_cpp} seconds")
        print(f"Total time taken for Python: {time_py} seconds")
        print(f"Total time saved by CPP: {round(time_py - time_cpp, precision)} seconds")
    
    # Save the simulation times for each topology to a CSV file
    simulation_times_df = pd.DataFrame.from_dict(simulation_times_per_topology, orient='index', columns=['Simulation Time CPP (Second)', 'Simulation Time Python (Second)'])
    simulation_times_df.index.name = 'Topology'

    # Save the DataFrame to a CSV file
    simulation_times_df.to_csv("results/summary_multiprocessing_cpp.csv", index=True)


def run_benchmark_multithreading():
    # save the simulation times for each topology
    simulation_times_per_topology = {}

    # topologies = [folder for folder in os.listdir('topologies') if
    #                          os.path.isdir(os.path.join('topologies', folder))]
    
    topologies = ["Abilene", "dfn-bwin", "Germany_17", "HiberniaUk", "polska"]

    for topology in topologies:
        print("===============Running benchmark multithreading for " + topology + "===============")

        # Load the topology
        print("Loading topology: " + topology)
        G, _, _ = read_graph('topologies/' + topology, topology)

        # Set the precision
        precision = 9
        print(f"Setting the comparsion precision to {precision}")
        
        # Create the availability dictionaries
        A_dic = {i:0.9 for i in range(G.number_of_nodes())}

        # Evaluate the availability with CPP
        time_start_cpp = time.time()
        calculate_availability_multithreading_cpp(G, A_dic)
        time_end_cpp = time.time()
        # Evaluate the availability with Python 
        time_start_py = time.time()
        calculate_availability_multithreading(G, A_dic)
        time_end_py = time.time()

        time_cpp = round(time_end_cpp - time_start_cpp, precision)
        time_py = round(time_end_py - time_start_py, precision)

        # Format the time taken
        formatted_time_cpp = f"{time_cpp:.{precision}f}"[:10]
        formatted_time_py = f"{time_py:.{precision}f}"[:10]

        # Save the simulation times for a overall comparison
        simulation_times_per_topology[topology] = (formatted_time_cpp, formatted_time_py)

        print(f"{topology} benchmark completed:")
        print(f"Total time taken for CPP: {time_cpp} seconds")
        print(f"Total time taken for Python: {time_py} seconds")
        print(f"Total time saved by CPP: {round(time_py - time_cpp, precision)} seconds")
    
    # Save the simulation times for each topology to a CSV file
    simulation_times_df = pd.DataFrame.from_dict(simulation_times_per_topology, orient='index', columns=['Simulation Time CPP (Second)', 'Simulation Time Python (Second)'])
    simulation_times_df.index.name = 'Topology'

    # Save the DataFrame to a CSV file
    simulation_times_df.to_csv("results/summary_multithreading_cpp.csv", index=True)


def run_benmark_with_mincutset():
    # read the mincutset from the file
    topologies = ["Germany_50", "Nobel_EU"]
    
    for topology in topologies:
        print("===============Running benchmark for " + topology + "===============")
        
        # Load the topology
        print("Loading topology: " + topology)
        G, _, _ = read_graph('topologies/' + topology, topology)
        
        # Create the availability dictionaries
        A_dic = {i:0.9 for i in range(G.number_of_nodes())}
        
        # Relabel the nodes of G and A_dic
        G, A_dic, relabel_mapping = relabel_graph_A_dict(G, A_dic)
        
        # Load the mincutsets
        print("Loading mincutsets: " + topology)
        df = pd.read_csv(f"topologies/{topology}/mincutsets_{topology}.csv")
        df['min-cutsets'] = df['min-cutsets'].apply(ast.literal_eval)
        mincutsets = df['min-cutsets'].values.tolist()
        
        # Relabel the mincutsets
        mincutsets_relabeled = [[[i + 1 for i in num] for num in sublist] for sublist in mincutsets]
        
        # Set the precision
        precision = 9
        print(f"Setting the comparsion precision to {precision}")
        
        # Get all the node pairs
        node_pairs = list(combinations(list(G.nodes), 2))
        
        result_data = []
        time_total_cpp = 0
        num_bool_expr_total = 0
        
        # Evaluate the availability with CPP
        for i in range(len(mincutsets_relabeled)):
            # Measure the CPP time taken
            time_start_cpp = time.time()
            try:
                build.rbd_bindings.evaluateAvailability(mincutsets_relabeled[i], A_dic, node_pairs[i][0], node_pairs[i][1])
            except Exception as e:
                print(e)
            time_end_cpp = time.time()
            
            time_cpp = round(time_end_cpp - time_start_cpp, precision)

            num_bool_expr = build.rbd_bindings.lengthOfProbaset(mincutsets_relabeled[i], node_pairs[i][0], node_pairs[i][1])
            num_bool_expr_total += num_bool_expr

            result_data.append({
                'source': node_pairs[i][0],
                'target': node_pairs[i][1],
                'CPP Time': time_cpp,
                'Number of Boolean Expressions': num_bool_expr
            })
            
            time_total_cpp += time_cpp
            
        time_total_cpp = round(time_total_cpp, precision)
        
        result_data.append({
            'source': 'All',
            'target': 'All',
            'CPP Time': time_total_cpp,
            'Number of Boolean Expressions': num_bool_expr_total
        })
        
        # Save the results to a CSV file for each topology
        result_df = pd.DataFrame(result_data)
        result_df.to_csv(f"results/{topology}_benchmark_with_mincutset.csv", float_format='%.9f', index=False)
        
        # Format the time taken
        formatted_time_cpp = f"{time_total_cpp:.{precision}f}"[:10]
        
        # Save the simulation times in the summary 
        with open("results/summary.csv", "a") as f:
            writer = csv.writer(f)
            writer.writerow([topology, formatted_time_cpp, "NA", num_bool_expr_total])
        
        print(f"{topology} benchmark completed:")
        print(f"Total time taken for CPP: {time_total_cpp} seconds")
        

if __name__ == "__main__":
    binding_cpp_files()
    run_test()
    run_benchmark()
    run_benmark_with_mincutset()
    run_benchmark_multiprocessing()
    run_benchmark_multithreading()
    run_benmark_with_mincutset()
    print("All tests and benchmarks completed")
    
        