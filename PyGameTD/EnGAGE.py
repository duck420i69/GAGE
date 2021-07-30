import atexit
import os
import glfw
import moderngl

class Window:
    def __init__(self, width, height, title):
        atexit.register(self.destroy)
        self.width = width
        self.height = height
        self.title = title
        self.window = None
        self.create()

    def create(self):
        glfw.init()
        glfw.window_hint(glfw.CONTEXT_VERSION_MAJOR, 3)
        glfw.window_hint(glfw.CONTEXT_VERSION_MINOR, 3)
        glfw.window_hint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE)
        self.window = glfw.create_window(self.width, self.height, self.title, None, None)
        glfw.make_context_current(self.window)
        print("Window created !")

    def destroy(self):
        glfw.destroy_window(self.window)
        glfw.terminate()
        print("Window destroyed !")

    def should_close(self):
        return glfw.window_should_close(self.window)

    def update(self):
        glfw.poll_events()

    def render(self):
        glfw.swap_buffers(self.window)

