import networkx as nx
from itertools import combinations, islice
import numpy as np






def main():
    G, data = read_graph_from_json('topologies/bridge_rbd_copy.json')

    minimal_cutsets = []
    for src in data["nodes"]:
        for dst in data["nodes"]:
            if src != dst:
                cutsets = minimalcuts(G, src, dst)
                if cutsets:
                    minimal_cutsets.append({
                        "src-dst": [src, dst],
                        "min-cutsets": cutsets
                    })

    data["minimal_cutsets"] = minimal_cutsets
    write_graph_to_json('topologies/bridge_rbd_test.json', G, data)
    # cutsets = minimalcuts(G, 0, 1)
    for cutset in minimal_cutsets:
        print(cutset)

if __name__ == '__main__':
    main()