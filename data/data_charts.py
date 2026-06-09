
import pandas as pd
import numpy as np
import tkinter as tk
from tkinter import filedialog, ttk
import matplotlib.pyplot as plt

# =========================
# DATA LOADING
# =========================

def choose_root_folder():
    root = tk.Tk()
    root.withdraw()
    folder = filedialog.askdirectory(title="Wybierz katalog z eksperymentami")
    if not folder:
        raise SystemExit("Nie wybrano katalogu.")
    return folder

def load_all_cars(root_folder):
    import os

    frames = []

    for dirpath, _, files in os.walk(root_folder):
        for file in files:
            if "cars" in file.lower() and file.lower().endswith(".csv"):
                full_path = os.path.join(dirpath, file)

                try:
                    df = pd.read_csv(full_path)

                    exp_name = os.path.basename(dirpath)
                    run_name = file.replace(".csv", "")

                    df["experiment"] = exp_name
                    df["run"] = run_name
                    df["source_file"] = full_path

                    frames.append(df)

                    print("Loaded:", full_path)

                except Exception as e:
                    print("Failed:", full_path, e)

    if not frames:
        raise ValueError("Nie znaleziono plików cars*.csv")

    return pd.concat(frames, ignore_index=True)

# =========================
# LOAD DATA
# =========================

root_folder = choose_root_folder()
df = load_all_cars(root_folder)

required = ["time", "id", "travel_id", "x", "y", "vx", "vy"]

for c in required:
    if c not in df.columns:
        raise ValueError(f"Brakuje kolumny {c}")

df["speed"] = np.sqrt(df["vx"]**2 + df["vy"]**2)

df = df.sort_values(
    ["experiment", "run", "travel_id", "id", "time"]
)

df["dx"] = df.groupby(
    ["experiment", "run", "travel_id", "id"]
)["x"].diff()

df["dy"] = df.groupby(
    ["experiment", "run", "travel_id", "id"]
)["y"].diff()

df["ds"] = np.sqrt(
    df["dx"]**2 +
    df["dy"]**2
)

df["distance"] = df.groupby(
    ["experiment", "run", "travel_id", "id"]
)["ds"].cumsum()

# =========================
# FILTER STATE
# =========================

selected_experiments = set()
selected_runs = set()
selected_travel_ids = set()
selected_ids = set()

# =========================
# FIGURES
# =========================

fig_dist, ax_dist = plt.subplots()
fig_speed, ax_speed = plt.subplots()
fig_traj, ax_traj = plt.subplots()
fig_flow, ax_flow = plt.subplots()
fig_density, ax_density = plt.subplots()
fig_fund, ax_fund = plt.subplots()
fig_heat, ax_heat = plt.subplots()

cross_x = -50
tolerance = 0.5
segment_min = -100
segment_max = -80

# =========================
# FILTERING
# =========================

def get_filtered_df():
    sub = df.copy()

    if selected_experiments:
        sub = sub[sub["experiment"].isin(selected_experiments)]

    if selected_runs:
        sub = sub[sub["run"].isin(selected_runs)]

    if selected_travel_ids:
        sub = sub[sub["travel_id"].isin(selected_travel_ids)]

    if selected_ids:
        sub = sub[sub["id"].isin(selected_ids)]

    return sub

# =========================
# PLOTS
# =========================

def update_charts():

    sub = get_filtered_df()

    if len(sub) == 0:
        print("Brak danych po filtracji.")
        return

    # DISTANCE
    ax_dist.clear()
    for _, g in sub.groupby(["experiment","run","travel_id","id"]):
        ax_dist.plot(g["time"], g["distance"])
    ax_dist.set_title("Distance")
    ax_dist.grid()

    # SPEED
    ax_speed.clear()
    for _, g in sub.groupby(["experiment","run","travel_id","id"]):
        ax_speed.plot(g["time"], g["speed"])
    ax_speed.set_title("Speed")
    ax_speed.grid()

    # TRAJ
    ax_traj.clear()
    for _, g in sub.groupby(["experiment","run","travel_id","id"]):
        ax_traj.plot(g["x"], g["y"])
    ax_traj.set_title("Trajectories")
    ax_traj.axis("equal")
    ax_traj.grid()

    # FLOW
    ax_flow.clear()

    tmp = sub.copy()
    tmp["cross"] = (
        (tmp["x"] > cross_x - tolerance) &
        (tmp["x"] < cross_x + tolerance)
    )

    flow = tmp[tmp["cross"]].groupby("time")["id"].nunique()

    if len(flow):
        flow.plot(ax=ax_flow)

    ax_flow.set_title("Flow")
    ax_flow.grid()

    # DENSITY
    ax_density.clear()

    tmp["in_segment"] = (
        (tmp["x"] >= segment_min) &
        (tmp["x"] <= segment_max)
    )

    density = (
        tmp[tmp["in_segment"]]
        .groupby("time")["id"]
        .nunique()
    )

    if len(density):
        density.plot(ax=ax_density)

    ax_density.set_title("Density")
    ax_density.grid()

    # FUNDAMENTAL
    ax_fund.clear()

    common = flow.index.intersection(
        density.index
    )

    if len(common):
        ax_fund.scatter(
            density.loc[common],
            flow.loc[common]
        )

    ax_fund.set_title("Fundamental Diagram")
    ax_fund.set_xlabel("density")
    ax_fund.set_ylabel("flow")
    ax_fund.grid()

    # HEATMAP
    ax_heat.clear()
    ax_heat.hist2d(
        sub["x"],
        sub["y"],
        bins=80
    )
    ax_heat.set_title("Heatmap")

    fig_dist.canvas.draw_idle()
    fig_speed.canvas.draw_idle()
    fig_traj.canvas.draw_idle()
    fig_flow.canvas.draw_idle()
    fig_density.canvas.draw_idle()
    fig_fund.canvas.draw_idle()
    fig_heat.canvas.draw_idle()

# =========================
# TREE WINDOW
# =========================

tree_root = tk.Tk()
tree_root.title("Simulation Filters")
tree_root.geometry("500x800")

tree = ttk.Treeview(tree_root)
tree.pack(fill="both", expand=True)

node_info = {}

def add_node(parent, text, node_type, value):
    item = tree.insert(
        parent,
        "end",
        text="☐ " + text,
        open=False
    )

    node_info[item] = {
        "type": node_type,
        "value": value,
        "checked": False
    }

    return item

# experiments

for exp in sorted(df["experiment"].unique()):

    exp_node = add_node(
        "",
        exp,
        "experiment",
        exp
    )

    exp_df = df[df["experiment"] == exp]

    for run in sorted(exp_df["run"].unique()):

        run_node = add_node(
            exp_node,
            run,
            "run",
            run
        )

        run_df = exp_df[exp_df["run"] == run]

        for travel_id in sorted(run_df["travel_id"].unique()):

            add_node(
                run_node,
                f"travel_id={travel_id}",
                "travel_id",
                int(travel_id)
            )

        for vehicle_id in sorted(run_df["id"].unique()):

            add_node(
                run_node,
                f"id={vehicle_id}",
                "id",
                int(vehicle_id)
            )

def set_checked(item, state):

    info = node_info[item]
    info["checked"] = state

    txt = tree.item(item, "text")[2:]

    tree.item(
        item,
        text=("☑ " if state else "☐ ") + txt
    )

    for child in tree.get_children(item):
        set_checked(child, state)

def rebuild_selection_sets():

    selected_experiments.clear()
    selected_runs.clear()
    selected_ids.clear()

    for item, info in node_info.items():

        if not info["checked"]:
            continue

        if info["type"] == "experiment":
            selected_experiments.add(info["value"])

        elif info["type"] == "run":
            selected_runs.add(info["value"])

        elif info["type"] == "travel_id":
            selected_travel_ids.add(info["value"])

        elif info["type"] == "id":
            selected_ids.add(info["value"])

def on_click(event):

    item = tree.identify_row(event.y)

    if not item:
        return

    current = node_info[item]["checked"]

    set_checked(item, not current)

tree.bind("<Double-1>", on_click)

def refresh():
    rebuild_selection_sets()
    update_charts()

def clear_all():
    for item in node_info:
        set_checked(item, False)

btn_frame = tk.Frame(tree_root)
btn_frame.pack(fill="x")

tk.Button(
    btn_frame,
    text="Refresh Charts",
    command=refresh
).pack(fill="x")

tk.Button(
    btn_frame,
    text="Reset Filters",
    command=clear_all
).pack(fill="x")

update_charts()

plt.show(block=False)
tree_root.mainloop()
