
import enum

class Tile:
    TILE_SIZE = 32
    
    def __init__(self, color):
        self.color = color

class TileType:
    NULL = Tile((0, 0, 0))
    GRASS = Tile((0, 137, 0))
    DIRT = Tile((192, 137, 0))

class Map:
    
    def __init__(self, width, height):
        self.width = width
        self.height = height
        self.map = []
    
    def load(self):
        for i in range(0, self.width * self.height):
            self.map.append(TileType.NULL)

    def update(self, window):
        for y in range(self.height):
            for x in range(self.width):

                #Render tile
                tile = self.map[x + y * self.width]
                window.draw_quad(
                    x * Tile.TILE_SIZE,
                    y * Tile.TILE_SIZE,
                    Tile.TILE_SIZE,
                    Tile.TILE_SIZE, tile.color[0], tile.color[1], tile.color[2]);

    