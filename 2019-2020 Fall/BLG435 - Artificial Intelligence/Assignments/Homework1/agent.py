

class Node:
    def __init__(self, position, parent=None):
        self.position = position  # position of player
        self.apples = set([])  # positions of apples

        self.path = ""   # move sequence
        self.f_cache = None  # avoid unnecessary calculations in f() function
        
        if parent:
            self.apples.update(parent.apples)

            # calculate the path (move sequence)
            if position[0] + 1 == parent.position[0]:
                self.path = parent.path + 'U'
            elif position[0] - 1 == parent.position[0]:
                self.path = parent.path +'D'
            elif position[1] + 1 == parent.position[1]:
                self.path = parent.path +'L'
            elif position[1] - 1 == parent.position[1]:
                self.path = parent.path +'R'

        self.apples.discard(self.position)  # remove the apple if an apple exist at the player position 
        self.depth = 0 if parent == None else parent.depth + 1  # update the depth value

    def __hash__(self):  # to represent nodes uniquely
        prime1, prime2 = 4111, 4973
        mod1, mod2 = 10**9 + 7, 10**9 + 9
        
        hash1 = self.position[0]
        hash2 = self.position[1]
        
        for apple in self.apples:
            hash1 = ((hash1 * prime1) + apple[0]) % mod1
            hash2 = ((hash2 * prime2) + apple[1]) % mod2

        return hash((hash1, hash2))

    def f(self):
        if self.f_cache:  
            return self.f_cache

        distance = float('-inf')  # holds Manhattan distance of farthest apple, initially minus infinity
        for apple in self.apples:
            distance = max(distance, Agent.manhattan_distance(apple, self.position))

        self.f_cache = self.depth + distance  # f(n) = g(n) + h(n), where g(n) is cost, h(n) is heuristic function
        return self.f_cache

    def __lt__(self, other):  # for heap operations
        return self.f() < other.f()


class Agent:
    def __init__(self):
        self.player_row = 0
        self.player_col = 0
        self.level_matrix = None
        self.elapsed_solve_time = 0
        self.directions = ((-1, 0), (1, 0), (0, -1), (0, 1))

        self.expanded_node_count = 0
        self.generated_node_count = 0
        self.maximum_node_in_memory_count = 0

    def solve(self, level_matrix, player_row, player_column):
        self.player_row = player_row
        self.player_col = player_column
        self.level_matrix = level_matrix

    @staticmethod
    def manhattan_distance(source, target):
        return abs(source[0] - target[0]) + abs(source[1] - target[1])
