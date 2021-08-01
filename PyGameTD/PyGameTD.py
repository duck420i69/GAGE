import pygame
import sys
from window import Window
from map import Map

SCREEN_WIDTH = 1600
SCREEN_HEIGHT = 900
SCREEN_TITLE = "Retarded tower defence"

def main():
    clock = pygame.time.Clock()
    win = Window(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE)
    map = Map(10, 10)

    map.load()
    
    while not win.close_requested():
        win.poll_events()
        delta = clock.tick(60)
        win.clear(0, 0, 0)
        
        #Game's logic and render here
        map.update(win)

        win.flip()

    win.shutdown()

if __name__ == "__main__":
    main()
