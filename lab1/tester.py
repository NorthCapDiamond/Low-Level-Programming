import pandas as pd
import matplotlib.pyplot as plt


df_add = pd.read_csv("add.csv")
df_delete = pd.read_csv("delete.csv")

plt.plot(df_add["index"], df_add["time"])
plt.title("Create Node")
plt.xlabel("Index")
plt.ylabel("Time")
plt.grid(True)
plt.show()


plt.plot(df_delete["index"], df_delete["time"])
plt.title("Delete Node")
plt.xlabel("Index")
plt.ylabel("Time")
plt.grid(True)
plt.show()