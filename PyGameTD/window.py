import sys, pygame

class Window:
    def __init__(self, width, height, title):
        pygame.init()
        self.width = width
        self.height = height
        self.title = title;
        self.surface = pygame.display.set_mode((width, height))
        pygame.display.set_caption(title)

        self.close_request = False

        #mouse_callback_fn(button, pos, down_state)
        self.mouse_callback_fn = None

        #key_callback_fn(key, down_state)
        self.key_callback_fn = None

        #mouse_pos_callback_fn(pos, rel, buttons)
        self.mouse_pos_callback_fn = None


    def shutdown(self):
        pygame.quit()

    def clear(self, r, g, b):
        self.surface.fill((r, g, b))
        
    def poll_events(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                self.close_request = True
            
            if self.mouse_pos_callback_fn != None:
                if event.type == pygame.MOUSEMOTION:
                    self.mouse_pos_callback_fn(event.pos, event.rel, event.buttons)

            if self.key_callback_fn != None:
                if event.type == pygame.KEYDOWN:
                    self.key_callback_fn(event.key, True)
                elif event.type == pygame.KEYUP:
                    self.key_callback_fn(event.key, False)

            if self.mouse_callback_fn != None:
                if event.type == pygame.MOUSEBUTTONDOWN:
                    self.mouse_callback_fn(event.button, event.pos, True)
                elif event.type == pygame.MOUSEBUTTONUP:
                    self.mouse_callback_fn(event.button, event.pos, False)
    
    def register_mouse_callback(self, callback):
        self.mouse_callback_fn = callback

    def register_mouse_pos_callback(self, callback):
        self.mouse_pos_callback_fn = callback

    def register_key_callback(self, callback):
        self.key_callback_fn = callback

    def close_requested(self) -> bool:
        return self.close_request
        
    def flip(self):   
        pygame.display.flip()
    
    def get_width(self):
        return self.width
    def get_height(self):
        return self.height
    def get_input(self):
        return self.input

    def draw_line(self, x, y, end_x, end_y, r = 255, g = 255, b = 255):
        pygame.draw.line(self.surface, (r, g, b), (x, y), (end_x, end_y), 1)

    def draw_quad(self, x, y, width, height, r = 255, g = 255, b = 255):
        pygame.draw.rect(self.surface, (r, g, b), (x, y, width, height), 0)

    def draw_circle(self, x, y, radius, r = 255, g = 255, b = 255):
        pygame.draw.circle(self.surface, (r, g, b), (x, y), radius)
    