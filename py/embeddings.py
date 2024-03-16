import torch
import xml.etree.ElementTree as ET
import numpy as np
import numpy
import sys
from rotary_embedding_torch import RotaryEmbedding

import itertools

def gray(n):
    return [n ^ (n >> 1)] if n else [0]

def cartesian_to_cyclic_gray(x, y):
    # Step 1: Convert Cartesian coordinates to polar coordinates
    r = (x**2 + y**2)**0.5
    theta = np.arctan2(y, x)
    
    # Step 2: Map polar coordinates to a 2D grid
    i = np.floor((theta + np.pi)/(2*np.pi)) * r
    j = np.floor((r - 1)/2)
    
    # Step 3: Encode the 2D grid into a cyclic Gray code
    return list(itertools.accumulate(gray(i), lambda x, y: (x << 1) | y)) + [0]

if __name__ == '__main__':
    # Load dataset
    dataset = sys.argv[1]
    tree = ET.parse(dataset)
    root = tree.getroot()
    nodes_e = root[1][0]
    nodes = np.zeros((len(nodes_e), 2))
    for node in nodes_e:
        nodes[int(node.attrib['id'])] = [float(node[0].text), float(node[1].text)]
    encoded_nodes = np.array([cartesian_to_cyclic_gray(x, y) for x, y in nodes])
    print(encoded_nodes)