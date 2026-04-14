import matplotlib.pyplot as plt

# Data
sizes = ["Nx=200 Ny=100", "Nx=800 Ny=400", "Nx=1600 Ny=800", "Nx=3200 Ny=1600"]

cpu_times = [0.230212, 3.00168, 15.9352, 75.6512]
gpu_times = [0.574674, 0.647328, 0.973897, 2.16255]

# Plot
plt.figure(figsize=(8, 5))

plt.plot(sizes, cpu_times, marker='D', linestyle='-', color='brown', label='CPU')
plt.plot(sizes, gpu_times, marker='D', linestyle='-', color='blue', label='GPU')

# Labels and styling
plt.xlabel("Grid Size (Nx, Ny)")
plt.ylabel("Runtime (seconds)")
plt.title("CPU vs GPU Runtime Scaling")
plt.grid(True)
plt.legend()

# Save figure
plt.savefig("runtime_compare.png", dpi=300, bbox_inches='tight')
