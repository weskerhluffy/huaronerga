'''
Created on 13/11/2016

@author: ernesto
https://teacode.wordpress.com/2013/08/02/algo-week-5-heap-and-dijkstras-shortest-path/
'''


import heapq
import logging
import sys

nivel_log = logging.ERROR
nivel_log = logging.DEBUG
logger_cagada = None

class PriorityQueue(object):
    """Priority queue based on heap, capable of inserting a new node with
    desired priority, updating the priority of an existing node and deleting
    an abitrary node while keeping invariant"""

    def __init__(self, heap=[]):
        """if 'heap' is not empty, make sure it's heapified"""

        heapq.heapify(heap)
        self.heap = heap
        self.entry_finder = dict({i[-1]: i for i in heap})
        logger_cagada.debug("el finder es %s" % self.entry_finder)
        self.REMOVED = sys.maxsize

    def insert(self, node, priority=0):
        """'entry_finder' bookkeeps all valid entries, which are bonded in
        'heap'. Changing an entry in either leads to changes in both."""

        if node in self.entry_finder:
            self.delete(node)
        entry = [priority, node]
        self.entry_finder[node] = entry
        heapq.heappush(self.heap, entry)
        logger_cagada.debug("el finde aora es %s" % self.entry_finder)
        logger_cagada.debug("el heap aora es %s" % self.heap)

    def delete(self, node):
        """Instead of breaking invariant by direct removal of an entry, mark
        the entry as "REMOVED" in 'heap' and remove it from 'entry_finder'.
        Logic in 'pop()' properly takes care of the deleted nodes."""

        logger_cagada.debug("norrando nodo %s" % node)
        entry = self.entry_finder.pop(node)
        entry[-1] = self.REMOVED
        return entry[0]

    def pop(self):
        """Any popped node marked by "REMOVED" does not return, the deleted
        nodes might be popped or still in heap, either case is fine."""

        while self.heap:
            logger_cagada.debug("elem de heap %s" % self.heap)
            priority, node = heapq.heappop(self.heap)
            if node is not self.REMOVED:
                del self.entry_finder[node]
                return priority, node
        raise KeyError('pop from an empty priority queue')


def dijkstra(source, pq, edges):
    """Returns the shortest paths from the source to all other nodes.
    'edges' are in form of {head: [(tail, edge_dist), ...]}, contain all
    edges of the graph, both directions if undirected."""

    processed = set()
#    uncharted = set([i[1] for i in pq.heap])
    shortest_path = {}
    pq.insert(source, 0)
    for mierda in edges.keys():
        if(mierda != source):
            pq.insert(mierda, sys.maxsize)
            
    size = len(pq.heap)
    while size > len(processed):
        logger_cagada.debug("size es %u len de proc %u" % (size, len(processed)))
        min_dist, new_node = pq.pop()
        logger_cagada.debug("mierda min %u node %u" % (min_dist, new_node))
        processed.add(new_node)
#        uncharted.remove(new_node)
        shortest_path[new_node] = min_dist
        for head, edge_dist in edges[new_node]:
            if head not in processed:
                old_dist = pq.delete(head)
                logger_cagada.debug("comparando old %u con %u mas %u", old_dist, min_dist , edge_dist)
                new_dist = min(old_dist, min_dist + edge_dist)
                pq.insert(head, new_dist)
        logger_cagada.debug("asta aora shotest %s" % shortest_path)
    return shortest_path




if __name__ == '__main__':
        FORMAT = "[%(filename)s:%(lineno)s - %(funcName)20s() ] %(message)s"
        logging.basicConfig(level=nivel_log, format=FORMAT)
        logger_cagada = logging.getLogger("asa")
        logger_cagada.setLevel(nivel_log)
        
        pq = PriorityQueue()
        puta_madre = {1:[(2, 3), (3, 4), (4, 2)], 2:[(1, 3), (3, 4), (5, 2)], 3:[(1, 4), (2, 4), (5, 6)], 4:[(1, 2), (5, 1), (6, 4)], 5:[(2, 2), (3, 6), (4, 1), (6, 2)], 6:[(4, 4), (5, 2)]}
        mierda = dijkstra(1, pq, puta_madre)
        logger_cagada.debug("carajo %s" % mierda)
