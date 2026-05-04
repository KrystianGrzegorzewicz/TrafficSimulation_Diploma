import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import RadioButtons, Button

# === DATA ===
df = pd.read_csv("cars.csv")

if "travel_id" not in df.columns:
    raise ValueError("Brakuje kolumny 'travel_id' w CSV!")

df["speed"] = np.sqrt(df["vx"]**2 + df["vy"]**2)
df = df.sort_values(["id", "time"])

df["dx"] = df.groupby("id")["x"].diff()
df["dy"] = df.groupby("id")["y"].diff()
df["ds"] = np.sqrt(df["dx"]**2 + df["dy"]**2)
df["distance"] = df.groupby("id")["ds"].cumsum()

travel_options = ["ALL"] + [str(t) for t in sorted(df["travel_id"].unique())]
current_index = 0

# === PARAMS ===
cross_x = -50
tolerance = 0.5
segment_min = -100
segment_max = -80

def get_subset(selected):
    if selected == "ALL":
        return df
    return df[df["travel_id"] == int(selected)]

# === CREATE FIGURES ===
fig_dist, ax_dist = plt.subplots()
fig_speed, ax_speed = plt.subplots()
fig_traj, ax_traj = plt.subplots()
fig_flow, ax_flow = plt.subplots()
fig_density, ax_density = plt.subplots()
fig_fund, ax_fund = plt.subplots()
fig_heat, ax_heat = plt.subplots()

# === CONTROL PANEL (tylko w jednym oknie, np. distance) ===
plt.figure(fig_dist.number)
plt.subplots_adjust(left=0.3)

rax = plt.axes([0.05, 0.5, 0.2, 0.3])
radio = RadioButtons(rax, travel_options)

ax_button = plt.axes([0.05, 0.4, 0.1, 0.05])
btn = Button(ax_button, 'Next')

# === UPDATE FUNCTION ===
def update(selected):
    sub = get_subset(selected).copy()

    # === DISTANCE ===
    ax_dist.clear()
    for _, g in sub.groupby("id"):
        ax_dist.plot(g["time"], g["distance"])
    ax_dist.set_title(f"Distance ({selected})")
    ax_dist.grid()
    fig_dist.canvas.draw_idle()

    # === SPEED ===
    ax_speed.clear()
    for _, g in sub.groupby("id"):
        ax_speed.plot(g["time"], g["speed"])
    ax_speed.set_title(f"Speed ({selected})")
    ax_speed.grid()
    fig_speed.canvas.draw_idle()

    # === TRAJ ===
    ax_traj.clear()
    for _, g in sub.groupby("id"):
        ax_traj.plot(g["x"], g["y"])
    ax_traj.set_title(f"Trajectories ({selected})")
    ax_traj.axis("equal")
    ax_traj.grid()
    fig_traj.canvas.draw_idle()

    # === FLOW ===
    sub["cross"] = (sub["x"] > cross_x - tolerance) & (sub["x"] < cross_x + tolerance)
    flow = sub[sub["cross"]].groupby("time")["id"].nunique()

    ax_flow.clear()
    flow.plot(ax=ax_flow)
    ax_flow.set_title(f"Flow ({selected})")
    ax_flow.grid()
    fig_flow.canvas.draw_idle()

    # === DENSITY ===
    sub["in_segment"] = (sub["x"] >= segment_min) & (sub["x"] <= segment_max)
    density = sub[sub["in_segment"]].groupby("time")["id"].nunique()

    ax_density.clear()
    density.plot(ax=ax_density)
    ax_density.set_title(f"Density ({selected})")
    ax_density.grid()
    fig_density.canvas.draw_idle()

    # === FUNDAMENTAL ===
    common_time = flow.index.intersection(density.index)

    ax_fund.clear()
    ax_fund.scatter(density.loc[common_time], flow.loc[common_time])
    ax_fund.set_title(f"Fundamental ({selected})")
    ax_fund.set_xlabel("density")
    ax_fund.set_ylabel("flow")
    ax_fund.grid()
    fig_fund.canvas.draw_idle()

    # === HEATMAP ===
    ax_heat.clear()
    h = ax_heat.hist2d(sub["x"], sub["y"], bins=80)
    ax_heat.set_title(f"Heatmap ({selected})")
    fig_heat.canvas.draw_idle()


# === EVENTS ===
def on_radio(label):
    global current_index
    current_index = travel_options.index(label)
    update(label)

def next_travel(event):
    global current_index
    current_index = (current_index + 1) % len(travel_options)
    radio.set_active(current_index)

radio.on_clicked(on_radio)
btn.on_clicked(next_travel)

# === INIT ===
update("ALL")

plt.show()