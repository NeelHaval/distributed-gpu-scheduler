import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path

# Create directory if it doesn't exits
RESULTS_DIR = Path(__file__).parent / "results"
RESULTS_DIR.mkdir(exist_ok=True)

# Benchmark data

runs = np.arange(1, 11)

makespan = np.array([
    27007.20,
    27429.48,
    27270.91,
    27219.06,
    27254.73,
    27125.93,
    27029.20,
    27321.84,
    27180.89,
    27097.46
])

average_job_time = np.array([
    1064.87,
    1069.44,
    1064.82,
    1067.38,
    1066.29,
    1065.71,
    1065.43,
    1067.14,
    1065.71,
    1064.41
])

throughput = np.array([
    3.70,
    3.65,
    3.67,
    3.67,
    3.67,
    3.69,
    3.70,
    3.66,
    3.68,
    3.69
])

utilisation = np.array([
    98.57,
    97.47,
    97.62,
    98.04,
    97.81,
    98.22,
    98.54,
    97.65,
    98.02,
    98.20
])

queueing_time = np.array([
    12788.72,
    13103.77,
    12974.78,
    12950.44,
    12981.10,
    12901.86,
    12804.92,
    13007.87,
    12907.03,
    12884.96
])

# Statistical summary

def print_summary(name, data, unit):
    mean = np.mean(data)
    std = np.std(data, ddof=1)
    minimum = np.min(data)
    maximum = np.max(data)

    print(f"\n{name}")
    print("-" * len(name))
    print(f"Mean:   {mean:.2f} {unit}")
    print(f"Std:    {std:.2f} {unit}")
    print(f"Min:    {minimum:.2f} {unit}")
    print(f"Max:    {maximum:.2f} {unit}")


print("========== BENCHMARK ANALYSIS ==========")
print("Workload: 100 jobs")
print("Workers: 4")
print("Scheduling algorithm: First-Fit")
print("Runs: 10")

print_summary("Makespan", makespan, "ms")
print_summary("Average Job Time", average_job_time, "ms")
print_summary("Throughput", throughput, "jobs/s")
print_summary("Worker Utilisation", utilisation, "%")
print_summary("Average Queueing Time", queueing_time, "ms")

# Coefficient of variation
# Measure of how much measurements vary compared with their average value

def coefficient_of_variation(data):
    return (np.std(data, ddof=1) / np.mean(data)) * 100


print("\n========== CONSISTENCY ==========")

print(
    f"Makespan variation: "
    f"{coefficient_of_variation(makespan):.2f}%"
)

print(
    f"Average job time variation: "
    f"{coefficient_of_variation(average_job_time):.2f}%"
)

print(
    f"Throughput variation: "
    f"{coefficient_of_variation(throughput):.2f}%"
)

print(
    f"Utilisation variation: "
    f"{coefficient_of_variation(utilisation):.2f}%"
)

print(
    f"Queueing time variation: "
    f"{coefficient_of_variation(queueing_time):.2f}%"
)

# Graph 1 (Makespan)

plt.figure()

plt.plot(
    runs,
    makespan,
    marker="o"
)

plt.axhline(
    np.mean(makespan),
    linestyle="--",
    label=f"Mean = {np.mean(makespan):.1f} ms"
)

plt.xlabel("Run")
plt.ylabel("Makespan (ms)")
plt.title("First-Fit Scheduler — Makespan Across 10 Runs")
plt.xticks(runs)
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig(RESULTS_DIR / "makespan.svg", bbox_inches="tight")
plt.close()

# Graph 2 (Throughput)

plt.figure()

plt.plot(
    runs,
    throughput,
    marker="o"
)

plt.axhline(
    np.mean(throughput),
    linestyle="--",
    label=f"Mean = {np.mean(throughput):.2f} jobs/s"
)

plt.xlabel("Run")
plt.ylabel("Throughput (jobs/s)")
plt.title("First-Fit Scheduler — Throughput Across 10 Runs")
plt.xticks(runs)
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig(RESULTS_DIR / "throughput.svg", bbox_inches="tight")
plt.close()

# Graph 3 (Worker Utilisation)

plt.figure()

plt.plot(
    runs,
    utilisation,
    marker="o"
)

plt.axhline(
    np.mean(utilisation),
    linestyle="--",
    label=f"Mean = {np.mean(utilisation):.2f}%"
)

plt.xlabel("Run")
plt.ylabel("Worker Utilisation (%)")
plt.title("First-Fit Scheduler — Worker Utilisation")
plt.xticks(runs)
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig(RESULTS_DIR / "utilisation.svg", bbox_inches="tight")
plt.close()

# Graph 4 (Queueing Time)

plt.figure()

plt.plot(
    runs,
    queueing_time,
    marker="o"
)

plt.axhline(
    np.mean(queueing_time),
    linestyle="--",
    label=f"Mean = {np.mean(queueing_time):.1f} ms"
)

plt.xlabel("Run")
plt.ylabel("Average Queueing Time (ms)")
plt.title("First-Fit Scheduler — Queueing Time")
plt.xticks(runs)
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig(RESULTS_DIR / "queueing_time.svg", bbox_inches="tight")
plt.close()

# Graph 5 (Metrics Normalised)
# Normalise metrics as this helps to compare them and highlights useful information
# about their relativity

metrics = {
    "Makespan": makespan,
    "Avg Job Time": average_job_time,
    "Throughput": throughput,
    "Utilisation": utilisation,
    "Queue Time": queueing_time
}

plt.figure()

for name, data in metrics.items():

    normalised = data / np.mean(data)

    plt.plot(
        runs,
        normalised,
        marker="o",
        label=name
    )

plt.axhline(
    1.0,
    linestyle="--"
)

plt.xlabel("Run")
plt.ylabel("Value relative to mean")
plt.title("First-Fit Scheduler — Relative Metric Variation")
plt.xticks(runs)
plt.legend()

plt.grid(True)
plt.tight_layout()
plt.savefig(RESULTS_DIR / "relative_metrics.svg", bbox_inches="tight")
plt.close()

# Graph 6 (Average Job Time)

plt.figure()

plt.plot(
    runs,
    average_job_time,
    marker="o"
)

plt.axhline(
    np.mean(average_job_time),
    linestyle="--",
    label=f"Mean = {np.mean(average_job_time):.2f} ms"
)

plt.xlabel("Run")
plt.ylabel("Average Job Execution Time (ms)")
plt.title("First-Fit Scheduler — Average Job Execution Time")
plt.xticks(runs)
plt.legend()
plt.grid(True)
plt.tight_layout()

plt.savefig(RESULTS_DIR / "average_job_time.svg", bbox_inches="tight")
plt.close()