import pygame, pypong
from pypong.player import BasicAIPlayer, KeyboardPlayer, MousePlayer, Rem0tePlayer
import r0ketrem0te.game
import time

class Pong:
    def __init__(self):
        self.configuration = {
            'screen_size': (686,488),
            'paddle_image': 'assets/paddle.png',
            'paddle_left_position': 84.,
            'paddle_right_position': 594.,
            'paddle_velocity': 6.,
            'paddle_bounds': (0, 488), # This sets the upper and lower paddle boundary.The original game didn't allow the paddle to touch the edge, 
            'line_image': 'assets/dividing-line.png',
            'ball_image': 'assets/ball.png',
            'ball_velocity': 4.,
            'ball_velocity_bounce_multiplier': 1.105,
            'ball_velocity_max': 32.,
            'score_left_position': (141, 30),
            'score_right_position': (473, 30),
            'digit_image': 'assets/digit_%i.png',
            'sound_missed': 'assets/missed-ball.wav',
            'sound_paddle': 'assets/bounce-paddle.wav',
            'sound_wall': 'assets/bounce-wall.wav',
            'sound': True,
        }
        pygame.mixer.pre_init(22050, -16, 2, 1024)
        pygame.init()

        self.rem0te = r0ketrem0te.game.Game('/dev/ttyACM0', "pong", 83, 81, (1,2,3,2,1), 2)
        self.rem0te.registerPlayerCallback(self.playercallback)

        self.player_right = Rem0tePlayer(self.rem0te)
        self.player_left  = Rem0tePlayer(self.rem0te)

        self.stop = True
        self.start = False
        self.restart()


    def playercallback(self, action, player):
        if action == 'added':
            if self.player_left.player == 0:
                self.player_left.player = player
            elif self.player_right.player == 0:
                self.player_right.player = player
            if self.player_left.player and self.player_right.player:
                self.start = True
        elif action == 'removed':
            if self.player_left.player == player:
                self.player_left.player = 0
            elif self.player_right.player == player:
                self.player_right.player = 0
            if self.player_left.player == 0 or self.player_right.player == 0:
                self.stop = True
    
    def restart(self):
        self.display_surface = pygame.display.set_mode(self.configuration['screen_size'])
        self.output_surface = self.display_surface.copy().convert_alpha()
        self.output_surface.fill((0,0,0))
        #~ debug_surface = output_surface.copy()
        #~ debug_surface.fill((0,0,0,0))
        self.debug_surface = None
        self.clock = pygame.time.Clock()
        self.input_state = {'key': None, 'mouse': None}
        
        # Prepare game

        self.game = pypong.Game(self.player_left, self.player_right, self.configuration)

    def run(self):
        # Main game loop
        timestamp = 1
        while self.game.running:
            if self.start:
                self.restart()
                self.start = False
                self.stop = False
            if self.stop:
                time.sleep(0.1)
                continue

            self.clock.tick(60)
            now = pygame.time.get_ticks()
            if timestamp > 0 and timestamp < now:
                timestamp = now + 5000
                print self.clock.get_fps()
            self.input_state['key'] = pygame.key.get_pressed()
            self.input_state['mouse'] = pygame.mouse.get_pos()
            self.game.update()
            self.game.draw(self.output_surface)
            #~ pygame.surfarray.pixels_alpha(output_surface)[:,::2] = 12
            self.display_surface.blit(self.output_surface, (0,0))
            if self.debug_surface:
                self.display_surface.blit(self.debug_surface, (0,0))
            pygame.display.flip()
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.game.running = False
                elif event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
                    self.game.running = False
            
if __name__ == '__main__':
    pong = Pong()
    pong.run()
