import pygame
import socket
import json
import time
import math

pygame.init()

# STARTOWY ROZMIAR (można zmienić, potem i tak jest dynamiczny)
WIDTH, HEIGHT = 1200, 800

screen = pygame.display.set_mode((WIDTH, HEIGHT), pygame.RESIZABLE)
pygame.display.set_caption("Simulation Viewer")
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

# ===== KAMERA =====

cam_x = 0
cam_y = 0
scale = 1.0
initialized = False

dragging = False
last_mouse = (0, 0)

# ===== AUTO FIT =====

def compute_bounds(cars, blocks, lines, circles):
    xs, ys = [], []

    for car in cars:
        xs.append(car["x"])
        ys.append(car["y"])

    for block in blocks:
        xs += [block["x1"], block["x2"]]
        ys += [block["y1"], block["y2"]]
    
    for line in lines:
        xs += [line["x1"], line["x2"]]
        ys += [line["y1"], line["y2"]]
    
    for circle in circles:
        xs += [circle["cx"] - circle["radius"], circle["cx"] + circle["radius"]]
        ys += [circle["cy"] - circle["radius"], circle["cy"] + circle["radius"]]

    if not xs:
        return 0, 0, 100, 100

    return min(xs), min(ys), max(xs), max(ys)

def fit_view(cars, blocks, lines, circles):
    global cam_x, cam_y, scale, WIDTH, HEIGHT

    min_x, min_y, max_x, max_y = compute_bounds(cars, blocks, lines, circles)

    world_w = max_x - min_x
    world_h = max_y - min_y

    if world_w == 0: world_w = 1
    if world_h == 0: world_h = 1

    padding = 100
    scale = min((WIDTH - padding) / world_w, (HEIGHT - padding) / world_h)

    cam_x = min_x + world_w / 2
    cam_y = min_y + world_h / 2

# ===== TRANSFORM =====

def to_screen(x, y):
    sx = (x - cam_x) * scale + WIDTH / 2
    sy = (y - cam_y) * scale + HEIGHT / 2
    return sx, sy

# ===== RYSOWANIE =====

def draw_car(x, y, vx, vy, ax, ay, color_r, color_g, color_b):
    car_length_m = 3.5
    car_width_m = 1.8

    width = max(2, car_length_m * scale)
    height = max(2, car_width_m * scale)

    angle = math.degrees(math.atan2(-vy, vx))
    car_surf = pygame.Surface((width, height), pygame.SRCALPHA)
    pygame.draw.rect(car_surf, (color_r, color_g, color_b), (0, 0, width, height))

    is_braking = (ax * vx + ay * vy) < 0
    light_color = (255, 40, 40) if is_braking else (90, 0, 0)

    light_w = width * 0.08
    light_h = height * 0.25
    margin = height * 0.1

    pygame.draw.rect(car_surf, light_color, (0, margin, light_w, light_h))
    pygame.draw.rect(car_surf, light_color, (0, height - margin - light_h, light_w, light_h))

    rotated = pygame.transform.rotate(car_surf, angle)
    rect = rotated.get_rect(center=(x, y))
    screen.blit(rotated, rect.topleft)

    pygame.draw.line(screen, (150, 50, 50), (x, y), (x + ax, y + ay), 4)
    pygame.draw.line(screen, (50, 200, 50), (x, y), (x + vx, y + vy), 2)

def draw_blocks():
    for block in blocks:
        x1, y1 = to_screen(block["x1"], block["y1"])
        x2, y2 = to_screen(block["x2"], block["y2"])
        color = (200, 0, 0) if block["active"] == 1 else (50, 50, 50)
        pygame.draw.rect(screen, color, (x1, y1, x2 - x1, y2 - y1))

def draw_lines():
    for line in lines:
        x1, y1 = to_screen(line["x1"], line["y1"])
        x2, y2 = to_screen(line["x2"], line["y2"])
        color = (line.get("r", 200), line.get("g", 200), line.get("b", 200))
        thickness = max(1, int(line.get("thickness", 1.0) * scale))
        pygame.draw.line(screen, color, (x1, y1), (x2, y2), thickness)

def draw_circles():
    for circle in circles:
        cx, cy = to_screen(circle["cx"], circle["cy"])
        radius = max(1, int(circle["radius"] * scale))
        color = (circle.get("r", 200), circle.get("g", 200), circle.get("b", 200))
        if circle.get("filled", False):
            pygame.draw.circle(screen, color, (int(cx), int(cy)), radius)
        else:
            pygame.draw.circle(screen, color, (int(cx), int(cy)), radius, 2)

# ===== MAIN =====

sock = connect()
buffer = ""

cars = []
blocks = []
lines = []
circles = []

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        # ⭐ NAJWAŻNIEJSZE — OBSŁUGA RESIZE OKNA
        elif event.type == pygame.VIDEORESIZE:
            WIDTH, HEIGHT = event.w, event.h
            screen = pygame.display.set_mode((WIDTH, HEIGHT), pygame.RESIZABLE)
            fit_view(cars, blocks, lines, circles)

        elif event.type == pygame.MOUSEWHEEL:
            zoom_factor = 1.1 if event.y > 0 else 0.9
            scale *= zoom_factor

        elif event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:
                dragging = True
                last_mouse = pygame.mouse.get_pos()

        elif event.type == pygame.MOUSEBUTTONUP:
            if event.button == 1:
                dragging = False

        elif event.type == pygame.MOUSEMOTION and dragging:
            mx, my = pygame.mouse.get_pos()
            dx = mx - last_mouse[0]
            dy = my - last_mouse[1]
            cam_x -= dx / scale
            cam_y -= dy / scale
            last_mouse = (mx, my)

    try:
        data = sock.recv(4096).decode()
        buffer += data
        while "\n" in buffer:
            line, buffer = buffer.split("\n", 1)
            world = json.loads(line)
            cars = world.get("cars", [])
            blocks = world.get("blocks", [])
            lines = world.get("lines", [])
            circles = world.get("circles", [])

            if not initialized:
                fit_view(cars, blocks, lines, circles)
                initialized = True

    except BlockingIOError:
        pass
    except Exception as e:
        print("Lost connection...", e)
        sock = connect()
        buffer = ""
        initialized = False

    screen.fill((30,30,30))

    draw_blocks()
    draw_lines()
    draw_circles()

    for car in cars:
        x, y = to_screen(car["x"], car["y"])
        vx = car["vx"] * scale
        vy = car["vy"] * scale
        ax = car["ax"] * scale
        ay = car["ay"] * scale
        draw_car(x, y, vx, vy, ax, ay, car["color_r"], car["color_g"], car["color_b"])

    pygame.display.flip()
    clock.tick(60)