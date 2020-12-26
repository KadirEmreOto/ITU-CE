from agent import Agent, Node
from itertools import permutations
from collections import deque


class DFSAgent(Agent):
    def __init__(self):
        super().__init__()

    def solve(self, level_matrix, player_row, player_column):
        super().solve(level_matrix, player_row, player_column)

        source = Node((player_row, player_column))
        self.generated_node_count = 1

        for i, row in enumerate(level_matrix):
            for j, cell in enumerate(row):
                if cell == "A":
                    source.apples.add((i, j))

        queue = deque([source])  # use as stack
        answer = None

        visited = {hash(source): source.depth}  # {state : solution length}

        while queue:
            self.maximum_node_in_memory_count = max(self.maximum_node_in_memory_count, len(queue))
            current = queue.pop()

            if answer and current.depth >= answer.depth:  # if current state is not better than the current answer, ignore
                del current
                continue

            flag = False
            for i, j in self.directions:
                x, y = current.position[0] + i, current.position[1] + j  # next position

                if  level_matrix[x][y] != 'W':
                    node = Node((x, y), current)
                    h = hash(node)

                    if h in visited and visited[h] <= node.depth:  # current state visited and not better that previous one 
                        del node
                        continue
                    
                    visited[h] = node.depth  # add to visited 

                    self.generated_node_count += 1
                    queue.append(node)
                    flag = True
                    
                    if not node.apples and (answer is None or answer.depth > node.depth):  # right part guarantees optimal solution
                        answer = node  # this may not be the optimal solution, so do not break, continue to search
                        
            if flag:
                self.expanded_node_count += 1

        return answer.path