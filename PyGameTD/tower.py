from random import randrange

class Bullet:
    BULLET_RADIUS = 8

    def __init__(self, d, s, x, y, v_x, v_y):
        self.d = d
        self.s = s
        self.x = x
        self.y = y
        self.v_x = v_x
        self.v_y = v_y
        self.dead = False

    def render(self, window):
        window.draw_circle(self.x, self.y, Bullet.BULLET_RADIUS, 255, 0, 0)

    def update(self, delta, window):
        self.x += self.v_x * delta * self.s
        self.y += self.v_y * delta * self.s

        #Check for out of bound/screen/display/something u see on the monitor bruh
        if self.x + Bullet.BULLET_RADIUS < 0.0 or self.y + Bullet.BULLET_RADIUS < 0.0 or self.x - Bullet.BULLET_RADIUS > window.get_width() or self.y - Bullet.BULLET_RADIUS > window.get_height():
            self.dead = True
    
    def is_dead(self):
        return self.dead;




class Tower:
    TOWER_SIZE = 32
    BULLET_SPEED = 0.1
    def __init__(self, atk, atks, rspd, x, y):
        self.atk = atk
        self.atks = atks
        self.rspd = rspd
        self.x = x 
        self.y = y
        self.accumulated_time = 0.0
        self.bullets = []
    
    def shoot(self):
        self.bullets.append(Bullet(self.atk, Tower.BULLET_SPEED, self.x * Tower.TOWER_SIZE + Tower.TOWER_SIZE / 2, self.y * Tower.TOWER_SIZE + Tower.TOWER_SIZE / 2, randrange(3) - 1, randrange(3) - 1))

    def render(self, window):
        window.draw_quad(self.x * Tower.TOWER_SIZE, self.y * Tower.TOWER_SIZE, Tower.TOWER_SIZE, Tower.TOWER_SIZE)
        for bullet in self.bullets:
            bullet.render(window)

    def update(self, delta, window):
        for x in range(0, len(self.bullets)):
            self.bullets[x].update(delta, window)
        for x in range(0, len(self.bullets)):
            if self.bullets[x].is_dead():
                del self.bullets[x]
                break;
        
        if self.atks == 0:
           return

        self.accumulated_time += delta;

        if self.accumulated_time >= self.atks:
            self.shoot()
            self.accumulated_time = 0.0

      
