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

# ===== TRANSFORMACJA ŚWIATA -> EKRAN =====

def compute_transform(cars, blocks):
    xs = []
    ys = []

    for car in cars:
        xs.append(car["x"])
        ys.append(car["y"])

    for block in blocks:
        xs += [block["x1"], block["x2"]]
        ys += [block["y1"], block["y2"]]

    if not xs:
        return 0, 0, 1

    min_x, max_x = min(xs), max(xs)
    min_y, max_y = min(ys), max(ys)

    world_w = max_x - min_x
    world_h = max_y - min_y

    if world_w == 0: world_w = 1
    if world_h == 0: world_h = 1

    padding = 50
    scale = min((WIDTH - padding) / world_w, (HEIGHT - padding) / world_h)

    return min_x, min_y, scale

def to_screen(x, y, min_x, min_y, scale):
    sx = (x - min_x) * scale
    sy = (y - min_y) * scale
    return sx, sy

# ===== RYSOWANIE =====

def draw_car(x, y, vx, vy):
    width, height = 40, 20

    angle = math.degrees(math.atan2(-vy, vx))

    car_surf = pygame.Surface((width, height), pygame.SRCALPHA)
    pygame.draw.rect(car_surf, (50, 100, 220), (0, 0, width, height))

    pygame.draw.rect(car_surf, (120, 0, 0), (0, 0, 4, 8))
    pygame.draw.rect(car_surf, (120, 0, 0), (0, height - 8, 4, 8))

    rotated = pygame.transform.rotate(car_surf, angle)
    rect = rotated.get_rect(center=(x, y))
    screen.blit(rotated, rect.topleft)

    pygame.draw.line(
        screen,
        (50, 200, 50),
        (x, y),
        (x + vx * 0.5, y + vy * 0.5),
        3
    )


def draw_blocks(min_x, min_y, scale):
    for block in blocks:
        x1, y1 = to_screen(block["x1"], block["y1"], min_x, min_y, scale)
        x2, y2 = to_screen(block["x2"], block["y2"], min_x, min_y, scale)

        pygame.draw.rect(
            screen,
            (100, 50, 50),
            (x1, y1, x2 - x1, y2 - y1)
        )

# ===== MAIN =====

sock = connect()
buffer = ""

cars = []
blocks = []

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

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
            blocks = world["blocks"]

    except BlockingIOError:
        pass
    except Exception as e:
        print("Lost connection...", e)
        sock = connect()
        buffer = ""

    # ===== TRANSFORM =====
    min_x, min_y, scale = compute_transform(cars, blocks)

    # ===== RYSOWANIE =====
    screen.fill((30,30,30))

    draw_blocks(min_x, min_y, scale)

    for car in cars:
        x, y = to_screen(car["x"], car["y"], min_x, min_y, scale)
        vx = car["vx"] * scale
        vy = car["vy"] * scale
        draw_car(x, y, vx, vy)

    pygame.display.flip()
    clock.tick(60)