from agent import Agent, Node
from itertools import permutations
from collections import deque


class BFSAgent(Agent):
    def __init__(self):
        super().__init__()

    def solve(self, level_matrix, player_row, player_column):
        super().solve(level_matrix, player_row, player_column)

        source = Node((player_row, player_column))
        self.generated_node_count = 1

        for i, row in enumerate(level_matrix):  # find apples' position
            for j, cell in enumerate(row):
                if cell == "A":
                    source.apples.add((i, j))

        queue = deque([source])  # use as queue
        answer = None
        visited = set([hash(source)])

        while queue and answer is None:
            self.maximum_node_in_memory_count = max(self.maximum_node_in_memory_count, len(queue))
            current = queue.popleft()

            if answer and current.depth >= answer.depth:  # actually it is not possible, first found answer should be optimal 
                del current
                continue

            flag = False
            for i, j in self.directions:
                x, y = current.position[0] + i, current.position[1] + j  # next position

                if level_matrix[x][y] != 'W':
                    node = Node((x, y), current)
                    h = hash(node)

                    if h in visited:  # current state visited before, ignore 
                        del node
                        continue
                    
                    visited.add(h)
                    self.generated_node_count += 1
                    queue.append(node)
                    flag = True
                    
                    if not node.apples:  # first final state should be the optimal one
                        answer = node
                        break
            if flag:
                self.expanded_node_count += 1

        return answer.path