import pygame
import sys
from random import randrange
from window import Window
from tower import *
from map import *

clock = pygame.time.Clock()
win = Window(1600, 900, "CUM")

towers = []

map = Map(50, 28)


while not win.close_requested():
    delta = clock.tick(60)
    win.poll_events()
    
    win.clear(0, 0, 0)
    
    map.render(win)
    for tower in towers:
        tower.render(win)
        tower.update(delta, win)
 
    win.flip()

