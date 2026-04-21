import pygame
import socket
import json
import time
import math

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

def draw_car(x, y, vx, vy):
    width, height = 40, 20

    # oblicz kąt z wektora prędkości
    angle = math.degrees(math.atan2(-vy, vx))

    # surface auta
    car_surf = pygame.Surface((width, height), pygame.SRCALPHA)

    # nadwozie – niebieskie
    pygame.draw.rect(car_surf, (50, 100, 220), (0, 0, width, height))

    # tylne światła (ciemnoczerwone)
    light_w, light_h = 4, 8
    light_y = (height - light_h) // 2

    # lewe tylne
    pygame.draw.rect(
        car_surf,
        (120, 0, 0),
        (0, 0, light_w, light_h)
    )

    # prawe tylne
    pygame.draw.rect(
        car_surf,
        (120, 0, 0),
        (0, light_y + light_h -1, light_w, light_h)
    )

    # obrót
    rotated = pygame.transform.rotate(car_surf, angle)
    rect = rotated.get_rect(center=(x, y))

    screen.blit(rotated, rect.topleft)

    # wektor prędkości
    pygame.draw.line(
        screen,
        (50, 200, 50),
        (x, y),
        (x + vx * 0.5, y + vy * 0.5),
        3
    )

def draw_lanes():
    for lane in lanes:
        pygame.draw.line(
            screen,
            (80,80,80),
            (lane["x1"], lane["y1"]),
            (lane["x2"], lane["y2"]),
            6
        )

def draw_blocks():
    for block in blocks:
        pygame.draw.rect(
            screen,
            (100, 50, 50),
            (block["x1"], block["y1"], block["x2"] - block["x1"], block["y2"] - block["y1"])
        )
sock = connect()
buffer = ""

cars = []
blocks = []
#lanes = []

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
            try:
                world = json.loads(line)
            except Exception as e:
                print("JSON ERROR:", e)
                print("BAD LINE:", line)
                continue
            cars = world["cars"]
            lanes = world["lanes"]
            blocks = world["blocks"]

    except BlockingIOError:
        pass
    except Exception as e:
        print("Lost connection...", e)
        sock = connect()
        buffer = ""

    # rysowanie
    screen.fill((30,30,30))
    draw_lanes()
    draw_blocks()
    for car in cars:
        draw_car(car["x"], car["y"], car["vx"], car["vy"])

    pygame.display.flip()
    clock.tick(60)