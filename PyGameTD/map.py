from random import randrange

class Tile:
	def __init__(self, r, g, b):
		self.r = r
		self.g = g
		self.b = b

class Map:
	TILE_SIZE = 32 # pixels
	def __init__(self, size_x, size_y):
		self.size_x = size_x
		self.size_y = size_y
		self.map = []

		for y in range(0, size_y):
			for x in range(0, size_x):
				self.map.append(Tile(56, 23, 75))

	def render(self, window):
		for x in range(0, self.size_x):
			for y in range(0, self.size_y):
				tile = self.map[x + y * self.size_x]
				window.draw_quad(x * Map.TILE_SIZE, y * Map.TILE_SIZE, Map.TILE_SIZE, Map.TILE_SIZE, tile.r, tile.g, tile.b)