import pygame
import socket
import json
import time

WIDTH, HEIGHT = 900, 600

pygame.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
clock = pygame.time.Clock()

def connect():
    while True:
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.connect(("127.0.0.1", 5555))
            s.setblocking(False)
            print("Connected to simulation!")
            return s
        except:
            print("Waiting for simulation...")
            time.sleep(1)

sock = connect()
buffer = ""

cars = []

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # odbiór danych z C++
    try:
        data = sock.recv(4096).decode()
        buffer += data

        while "\n" in buffer:
            line, buffer = buffer.split("\n", 1)
            world = json.loads(line)
            cars = world["cars"]

    except BlockingIOError:
        pass
    except:
        print("Lost connection...")
        sock = connect()

    # rysowanie
    screen.fill((30,30,30))

    for car in cars:
        x, y = car["x"], car["y"]
        vx, vy = car["vx"], car["vy"]

        # auto
        pygame.draw.rect(screen, (200,50,50), (x, y, 40, 20))

        # wektor prędkości
        pygame.draw.line(screen, (50,200,50), (x+20, y+10),
                         (x+20 + vx*0.5, y+10 + vy*0.5), 3)

    pygame.display.flip()
    clock.tick(60)