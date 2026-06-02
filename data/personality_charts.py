import pandas as pd
import matplotlib.pyplot as plt
from tkinter import filedialog
import tkinter as tk

root = tk.Tk()
root.withdraw()

file = filedialog.askopenfilename()

df = pd.read_csv(file)

plt.figure()
plt.hist(df["aggression"], bins=30)
plt.title("Aggression")

plt.figure()
plt.hist(df["gap_factor"], bins=30)
plt.title("Gap Factor")

plt.figure()
plt.hist(df["reaction_factor"], bins=30)
plt.title("Reaction Factor")

plt.figure()
plt.scatter(
    df["aggression"],
    df["gap_factor"]
)
plt.xlabel("Aggression")
plt.ylabel("Gap")

plt.show()