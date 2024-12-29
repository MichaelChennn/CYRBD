import multiprocessing
import os
import pickle as pkl
from itertools import combinations
import networkx as nx
from mainAvailability import calculate_availability
import pandas as pd
import time


def evaluate_availability_single(G, source, target, A_dic):
    try:
        node_labels = nx.get_node_attributes(G, 'label')
    except:
        node_labels = nx.get_node_attributes(G, 'name')

    city_labels = {i: label for i, label in enumerate(node_labels.values())}
    G = nx.relabel_nodes(G, city_labels)

    result, combined_results = calculate_availability(G, source, target, A_dic)

    if isinstance(A_dic, list):
        result_df = pd.DataFrame(result, columns=['source', 'target', *[f'{val} Availability' for val in A_dic]])
    elif isinstance(A_dic, dict):
        result_df = pd.DataFrame(result, columns=['source', 'target', ' Availability'])
    result_df.to_csv('availability_evaluation.csv', index=False)

    return result


def process_pair(pair, G, A_dic):
    source_node, target_node = pair
    process_id = os.getpid()
    #print(f"Processing pair {pair} in process {process_id}")
    start_time = time.time()
    result, combined_results = calculate_availability(G, source_node, target_node, A_dic)
    end_time = time.time()
    pair_simulation_time = end_time - start_time
    return pair, result, pair_simulation_time


def evaluate_availability_multiple(G, R, A_dic):
    if 'label' in G.nodes[0]:
        node_labels = nx.get_node_attributes(G, 'label')
    elif 'name' in G.nodes[0]:
        node_labels = nx.get_node_attributes(G, 'name')

    # print(node_labels)

    # print(node_labels)
    city_labels = {i: label for i, label in enumerate(node_labels.values())}
    G = nx.relabel_nodes(G, city_labels)

    adjusted_pairs = [(city_labels[pair[0]], city_labels[pair[1]]) for pair in R]

    result_accumulator = []
    simulation_times_per_pair = {}

    num_cpus = multiprocessing.cpu_count()
    print("Number of CPUs available:", num_cpus)

    pool = multiprocessing.Pool(processes=num_cpus)

    results = pool.starmap(process_pair, [(pair, G, A_dic) for pair in adjusted_pairs])

    pool.close()
    pool.join()

    for pair, result, pair_simulation_time in results:
        result_accumulator.extend(result)
        simulation_times_per_pair[pair] = pair_simulation_time

    # print('ssttimperdm',simulation_times_per_pair)

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

    simulation_times_per_folder = {}
    simulation_times_demand = {}
    all_simulation_times_per_folder = {}

    topologies_path = os.path.join(os.getcwd(), 'topologies')

    folders_in_topologies = [folder for folder in os.listdir(topologies_path) if
                             os.path.isdir(os.path.join(topologies_path, folder))]

    for folder in folders_in_topologies:
        folder_path = os.path.join(topologies_path, folder)
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
        print('Topology: ' + str(folder) + ' , time with multiprocessing:', simtime)
        total_simulation_times_per_demand = {}

        for pair, demand_simulation_time in total_simulation_time.items():
            total_simulation_times_per_demand[pair] = demand_simulation_time

        all_simulation_times_per_folder[folder] = total_simulation_times_per_demand

        # Save simulation time
        simulation_time_df = pd.DataFrame({'Simulation Time (seconds)': [simtime]})
        simulation_time_df.to_csv('simulation_time_multiprocessing.csv', index=False)

        # Simulation time for each topology
        simulation_times.append({'Topology': folder, 'Simulation Time (seconds)': simtime})

        df_times = pd.DataFrame(simulation_times)
        df_times.to_excel(os.path.join(os.getcwd(), 'simulation_times_multiprocessing.xlsx'), index=False)

        df = pd.DataFrame(result_accumulator,
                          columns=['source', 'target', *[f'{val} Availability' for val in A_dic]])

        excel_file_path = os.path.join(folder_path, f'{folder}_availability.xlsx')
        with pd.ExcelWriter(excel_file_path, engine='openpyxl') as excel_writer:
            df.to_excel(excel_writer, sheet_name=folder, index=False)
            excel_writer.sheets[folder]

    excel_writer = pd.ExcelWriter('simulation_times_per_demand_multiprocessing.xlsx', engine='xlsxwriter')

    for folder, total_simulation_times_per_demand in all_simulation_times_per_folder.items():
        df = pd.DataFrame(total_simulation_times_per_demand.items(), columns=['Demand', 'Total Simulation Time'])

        df.to_excel(excel_writer, sheet_name=folder, index=False)

    excel_writer.close()
    
    
    
    
