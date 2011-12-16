import pygame, math
from pygame.sprite import Sprite
    
class Paddle(Sprite):
    def __init__(self, velocity, image, bounds_y, *groups):
        Sprite.__init__(self, *groups)
        self.image = image
        self.rect = self.image.get_rect()
        self.direction = 0
        self.velocity = velocity
        self.bounds_y = bounds_y
        # Like original pong, we break this up into 8 segments from the edge angle (acute_angle) to pi/2 at the center
        # Changing acute_angle lets us change the extreme edge angle of the paddle.
        acute_angle = .125
        # Build the angles from acute_angle to the first 0.5 center value then append the values going from the
        # second center 0.5 value by using the values we just calculated reversed.
        angles = [acute_angle + (0.5-acute_angle)/3.0 * n for n in xrange(4)]
        angles += map(lambda x: 1 + x * -1, reversed(angles))
        # Final table is the output vector (x,y) of each angle
        self.bounce_table = [(math.cos(n*math.pi-math.pi/2.0), math.sin(n*math.pi-math.pi/2.0)) for n in angles]
        
    def update(self):
        self.rect.y = max(self.bounds_y[0], min(self.bounds_y[1]-self.rect.height, self.rect.y + self.direction * self.velocity))

    def calculate_bounce(self, delta):
        return self.bounce_table[int(round(delta * (len(self.bounce_table)-1)))]
    
class Line(Sprite):
    def __init__(self, image, *groups):
        Sprite.__init__(self, *groups)
        self.image = image
        self.rect = self.image.get_rect()

class Ball(Sprite):
    def __init__(self, velocity, image, *groups):
        Sprite.__init__(self, *groups)
        self.velocity = velocity
        self.image = image
        self.rect = self.image.get_rect()
        self.position_vec = [0., 0.]
        self.velocity_vec = [0., 0.]
        
    def update(self):
        self.position_vec[0] += self.velocity_vec[0]
        self.position_vec[1] += self.velocity_vec[1]
        self.rect.x = self.position_vec[0]
        self.rect.y = self.position_vec[1]
    
    def set_position_x(self, value):
        self.position_vec[0] = value
        self.rect.left = value
    position_x = property(lambda self: self.position_vec[0], set_position_x)
        
    def set_position_y(self, value):
        self.position_vec[1] = value
        self.rect.top = value
    position_y = property(lambda self: self.position_vec[1], set_position_y)
    
class Score(Sprite):
    def __init__(self, image_list, *groups):
        Sprite.__init__(self, *groups)
        self.image_list = image_list
        self.image = None
        self.rect = pygame.Rect(0,0,0,0)
        self.score = 0
    
    def get_score(self):
        return self.score_value
        
    def set_score(self, value):
        self.score_value = value
        digit_spacing = 8
        digit_width = self.image_list[0].get_width()
        digit_height = self.image_list[0].get_height()
        values = map(int, reversed(str(self.score_value)))
        surface_width = len(values) * digit_width + (len(values)-1) * digit_spacing
        if not self.image or self.image.get_width() < surface_width:
            self.image = pygame.Surface((surface_width, digit_height))
            self.image.fill((0,0,0))
            self.rect.width = self.image.get_width()
            self.rect.height = self.image.get_height()
        offset = self.image.get_width()-digit_width
        for i in values:
            self.image.blit(self.image_list[i], (offset, 0))
            offset = offset - (digit_width + digit_spacing)
        
    score = property(get_score, set_score)
