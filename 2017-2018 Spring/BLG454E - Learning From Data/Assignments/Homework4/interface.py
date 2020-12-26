import sys
import pygame
from random import randint

pygame.init()
colors = {'bg': (40, 44, 52), 'text': (254, 211, 63)}


class InterFace(object):
    def __init__(self):
        self.size = (500, 500)
        self.screen = pygame.display.set_mode(self.size)
        self.screen.fill(colors['bg'])

        self.means = []
        self.points = []
        self.visited = set()
        self.add_random_points(number=5)
        self.add_random_means(number=5)

    def solve(self):
        self.group()
        self.move_means()

    @staticmethod
    def distance(u, v):
        return (u[0] - v[0]) ** 2 + (u[1] - v[1]) ** 2

    def group(self):
        for m in self.means:
            m[1] = []

        for point in self.points:
            dist = float('inf')
            mean = None

            for m in self.means:
                d = self.distance(m[0], point)
                if d < dist:
                    dist = d
                    mean = m

            mean[1].append(point)

        for point, array, color in self.means:
            for x, y in array:
                self.draw(x, y, color)

    def move_means(self):
        for mean in self.means:
            self.draw(mean[0][0], mean[0][1], color=colors['bg'])
            mean[0] = self.find_center(mean)
            self.draw(mean[0][0], mean[0][1], mean[2], flag=True)

    @staticmethod
    def find_center(mean):
        if not mean[1]:
            return mean[0]

        x = 0
        y = 0

        for p in mean[1]:
            x += p[0]
            y += p[1]

        x /= len(mean[1])
        y /= len(mean[1])

        return [int(x), int(y)]

    def add_random_points(self, number=20):
        for i in range(number):
            x = randint(0, 500)
            y = randint(0, 500)

            self.add_new_point(x, y)

    def add_random_means(self, number=5):
        for i in range(number):
            x = randint(0, 500)
            y = randint(0, 500)
            c = self.color_generate()
            self.means.append([(x, y), [], c])

            self.draw(x, y, c, flag=True)

    def add_new_point(self, x, y, color=colors['text'], size=5):
        if (x, y) in self.visited:
            return

        self.visited.add((x, y))
        self.draw(x, y, color, size)
        self.points.append((x, y))

    def draw(self, x, y, color=colors['text'], size=5, flag=False):
        for i in range(x - size // 2, x + size // 2 + 1):
            for j in range(y - size // 2, y + size // 2 + 1):
                self.screen.set_at((i, j), color)

        if flag:
            for i in range(x - size // 2, x + size // 2 + 1):
                self.screen.set_at((i, y - size // 2), (255, 255, 255))
                self.screen.set_at((i, y + size // 2), (255, 255, 255))

            for j in range(y - size // 2, y + size // 2 + 1):
                self.screen.set_at((x - size // 2, j), (255, 255, 255))
                self.screen.set_at((x + size // 2, j), (255, 255, 255))

    def clear(self):
        self.screen.fill(colors['bg'])
        self.points = []

    @staticmethod
    def color_generate():
        return randint(0, 255), randint(0, 255), randint(0, 255)

    def main(self):
        pygame.display.flip()

        while True:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    pygame.quit()
                    sys.exit()

                if pygame.mouse.get_pressed()[0]:
                    try:
                        self.add_new_point(*event.pos)
                        pygame.display.flip()
                    except Exception as e:
                        print(e)

                if event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_c:
                        self.clear()
                        pygame.display.flip()

                if event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_g:
                        self.group()
                        pygame.display.flip()

                if event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_m:
                        self.move_means()
                        pygame.display.flip()

                if event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_s:
                        self.solve()
                        pygame.display.flip()

                if event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_q:
                        pygame.quit()
                        sys.exit()

if __name__ == '__main__':
    interface = InterFace()
    interface.main()
