# -*- coding: utf-8 -*-
"""
pydot example 1
@author: Federico Cáceres
@url: http://pythonhaven.wordpress.com/2009/12/09/generating_graphs_with_pydot
"""
import pydot # import pydot or you're not going to get anywhere my friend :D
graph = pydot.graph_from_dot_file('factorGraphTopology.txt')
graph[0].write_png('example1_graph.png')
