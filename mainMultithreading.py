import os
import pickle as pkl
import threading
from itertools import combinations
import networkx as nx
from mainAvailability import calculate_availability
import pandas as pd
import time


def evaluate_availability_single(G, source, target, A_dic):

    node_labels = nx.get_node_attributes(G, 'label')
    city_labels = {i: label for i, label in enumerate(node_labels.values())}
    G = nx.relabel_nodes(G, city_labels)

    result, combined_results = calculate_availability(G, source, target, A_dic)

    # Save result
    if isinstance(A_dic, list):
        result_df = pd.DataFrame(result, columns=['source', 'target', *[f'{val} Availability' for val in A_dic]])
    elif isinstance(A_dic, dict):
        result_df = pd.DataFrame(result, columns=['source', 'target', ' Availability'])
    result_df.to_csv('availability_evaluation.csv', index=False)

def evaluate_availability_multiple(G, R, A_dic):

    # node_labels = nx.get_node_attributes(G, 'label')
    # city_labels = {i: label for i, label in enumerate(node_labels.values())}
    # G = nx.relabel_nodes(G, city_labels)

    if 'label' in G.nodes[0]:
        node_labels = nx.get_node_attributes(G, 'label')
    elif 'name' in G.nodes[0]:
        node_labels = nx.get_node_attributes(G, 'name')

    #print(node_labels)

    # print(node_labels)
    city_labels = {i: label for i, label in enumerate(node_labels.values())}
    G = nx.relabel_nodes(G, city_labels)

    adjusted_pairs = [(city_labels[pair[0]], city_labels[pair[1]]) for pair in R]

    result_accumulator = []
    # simulation_times = {}
    # total_simulation_time = 0

    #print('All paris in graph', adjusted_pairs)


    # Function to process each pair
    def process_pair(pair, result_dict):
        source_node, target_node = pair
        start_time = time.time()  # Start time for the pair
        result, combined_results = calculate_availability(G, source_node, target_node, A_dic)
        end_time = time.time()  # End time for the pair
        pair_simulation_time = end_time - start_time  # Simulation time for the pair
        result_dict[pair] = (result, pair_simulation_time)  # Store result and si


    # Dictionary to store the results of each thread
    thread_results = {}

    # Create threads for each pair and start them
    threads = []
    for pair in adjusted_pairs:
        thread_results[pair] = None  # Initialize result placeholder
        thread = threading.Thread(target=process_pair, args=(pair, thread_results))
        threads.append(thread)
        thread.start()

    # Wait for all threads to finish
    for thread in threads:
        thread.join()

    simulation_times_per_pair = {}
    # Collect results and calculate total simulation time
    for pair, (result, pair_simulation_time) in thread_results.items():
        result_accumulator.extend(result)
        simulation_times_per_pair[pair] = pair_simulation_time
    #print('ssstttt',simulation_times_per_pair)
    return result_accumulator, simulation_times_per_pair

def read_graph(directory, tt):
    with open(os.path.join(directory, 'Pickle_' + tt + '.pickle'), 'rb') as handle:
        f = pkl.load(handle)
    G = f[0]
    pos = f[1]
    lable = f[2]
    return G, pos, lable


if __name__ == "__main__":

    A_dic = [0.9, 0.99, 0.999, 0.9999]
    simulation_times = []
    # simulation_memory = []
    simulation_times_per_folder = {}
    simulation_times_demand = []
    all_simulation_times_per_folder = {}

    topologies_path = os.path.join(os.getcwd(), 'topologies')
    # List all folders in the "topologies" folder
    folders_in_topologies = [folder for folder in os.listdir(topologies_path) if
                             os.path.isdir(os.path.join(topologies_path, folder))]

    for folder in folders_in_topologies:
        folder_path = os.path.join(topologies_path, folder)
        # Loading the graph and its positions from the pickle file
        G, pos, lable = read_graph(folder_path, folder)

        R = list(combinations(list(G.nodes), 2))

        strat_time = time.time()
        # tracemalloc.start()
        result_accumulator, total_simulation_time = evaluate_availability_multiple(G, R, A_dic)
        # current, peak = tracemalloc.get_traced_memory()  # Get memory usage statistics
        # Stop tracing memory allocations
        # tracemalloc.stop()
        end = time.time()
        simtime = end - strat_time
        print('toplogy: '  + str(folder) + 'time with multithreading:', simtime)


        total_simulation_times_per_demand = {}

        # Save the total simulation time per demand in the dictionary
        for pair, demand_simulation_time in total_simulation_time.items():
            total_simulation_times_per_demand[pair] = demand_simulation_time

        # Store the total simulation times per demand for the current folder
        all_simulation_times_per_folder[folder] = total_simulation_times_per_demand

        # Save result
        if isinstance(A_dic, list):
            result_df = pd.DataFrame(result_accumulator,
                                     columns=['source', 'target', *[f'{val} Availability' for val in A_dic]])
        elif isinstance(A_dic, dict):
            result_df = pd.DataFrame(result_accumulator, columns=['source', 'target', ' Availability'])
        result_df.to_csv('availability_evaluation_multithreading.csv', index=False)

        # Save simulation time
        simulation_time_df = pd.DataFrame({'Simulation Time (seconds)': [simtime]})
        simulation_time_df.to_csv('simulation_time_multithreading.csv', index=False)

        # Simulation time for each topology
        simulation_times.append({'Topology': folder, 'Simulation Time (seconds)': simtime})

        df_times = pd.DataFrame(simulation_times)
        df_times.to_excel(os.path.join(os.getcwd(), 'simulation_times_multithreading.xlsx'), index=False)


    # Create a Pandas Excel writer using XlsxWriter as the engine
    excel_writer = pd.ExcelWriter('simulation_times_per_demand_multithreading.xlsx', engine='xlsxwriter')

    # Loop through each folder and write total simulation times per demand to a sheet
    for folder, total_simulation_times_per_demand in all_simulation_times_per_folder.items():
        # Convert the dictionary to a DataFrame
        df = pd.DataFrame(total_simulation_times_per_demand.items(), columns=['Demand', 'Total Simulation Time'])

        # Write DataFrame to a sheet in the Excel file
        df.to_excel(excel_writer, sheet_name=folder, index=False)

    # Save the Excel file
    excel_writer.close()

    print("Total simulation times per demand per folder saved to 'total_simulation_times_per_demand_multithreading.xlsx'")

