import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path

# Create directory if it doesn't exits
RESULTS_DIR = Path(__file__).parent / "results"
RESULTS_DIR.mkdir(exist_ok=True)

# Benchmark data (first fit)

runs = np.arange(1, 11)

first_fit_makespan = np.array([
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

first_fit_average_job_time = np.array([
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

first_fit_throughput = np.array([
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

first_fit_utilisation = np.array([
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

first_fit_queueing_time = np.array([
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

# Benchmark data (Best fit)

best_fit_makespan = np.array([
    15044.28,
    15401.33,
    15161.62,
    16638.53,
    16829.05,
    15606.52,
    15985.24,
    16032.69,
    16357.57,
    16648.69
])

best_fit_average_job_time = np.array([
    983.84,
    984.09,
    975.33,
    915.31,
    879.64,
    981.99,
    956.61,
    940.34,
    946.72,
    1043.89
])

best_fit_throughput = np.array([
    6.65,
    6.49,
    6.60,
    6.01,
    5.94,
    6.41,
    6.26,
    6.24,
    6.11,
    6.01
])

best_fit_cpu_utilisation = np.array([
    79.68,
    78.00,
    78.45,
    66.52,
    64.33,
    76.64,
    72.36,
    70.01,
    69.19,
    76.79
])

best_fit_gpu_utilisation = np.array([
    16.35,
    15.97,
    16.08,
    13.75,
    13.07,
    15.73,
    14.96,
    14.66,
    14.47,
    15.68
])

best_fit_memory_utilisation = np.array([
    23.23,
    22.74,
    22.86,
    19.31,
    18.92,
    22.35,
    21.12,
    20.29,
    20.00,
    22.36
])

best_fit_queueing_time = np.array([
    5567.23,
    5668.63,
    5762.13,
    6936.54,
    7470.55,
    5615.94,
    6993.70,
    6901.83,
    6893.84,
    6085.89
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

# Coefficient of variation
# Measure of how much measurements vary compared with their average value

def coefficient_of_variation(data):
    return (np.std(data, ddof=1) / np.mean(data)) * 100


print("========== FIRST FIT BENCHMARK ANALYSIS ==========")
print("Workload: 100 jobs")
print("Workers: 4")
print("Scheduling algorithm: First-Fit")
print("Runs: 10")

print_summary("first_fit_Makespan", first_fit_makespan, "ms")
print_summary("first_fit_Average Job Time", first_fit_average_job_time, "ms")
print_summary("first_fit_Throughput", first_fit_throughput, "jobs/s")
print_summary("first_fit_Worker Utilisation", first_fit_utilisation, "%")
print_summary("first_fit_Average Queueing Time", first_fit_queueing_time, "ms")


print("\n========== FIRST FIT CONSISTENCY ==========")

print(
    f"Makespan variation: "
    f"{coefficient_of_variation(first_fit_makespan):.2f}%"
)

print(
    f"Average job time variation: "
    f"{coefficient_of_variation(first_fit_average_job_time):.2f}%"
)

print(
    f"Throughput variation: "
    f"{coefficient_of_variation(first_fit_throughput):.2f}%"
)

print(
    f"Utilisation variation: "
    f"{coefficient_of_variation(first_fit_utilisation):.2f}%"
)

print(
    f"Queueing time variation: "
    f"{coefficient_of_variation(first_fit_queueing_time):.2f}%"
)

print("\n========== UPGRADED SCHEDULER BENCHMARK ANALYSIS ==========")
print("Workload: 100 jobs")
print("Workers: 4")
print("Runs: 10")

print_summary("Makespan", best_fit_makespan, "ms")

print_summary("Average Job Time", best_fit_average_job_time, "ms")

print_summary("Throughput", best_fit_throughput, "jobs/s")

print_summary("CPU Utilisation", best_fit_cpu_utilisation, "%")

print_summary("GPU Utilisation", best_fit_gpu_utilisation, "%"
)

print_summary("Memory Utilisation", best_fit_memory_utilisation, "%")

print_summary("Average Queueing Time", best_fit_queueing_time, "ms")

print("\n========== UPGRADED SCHEDULER CONSISTENCY ==========")

print(f"Makespan variation: " f"{coefficient_of_variation(best_fit_makespan):.2f}%")

print(f"Average job time variation: " f"{coefficient_of_variation(best_fit_average_job_time):.2f}%")

print(f"Throughput variation: " f"{coefficient_of_variation(best_fit_throughput):.2f}%")

print(f"CPU utilisation variation: " f"{coefficient_of_variation(best_fit_cpu_utilisation):.2f}%")

print(f"GPU utilisation variation: " f"{coefficient_of_variation(best_fit_gpu_utilisation):.2f}%")

print(f"Memory utilisation variation: " f"{coefficient_of_variation(best_fit_memory_utilisation):.2f}%")

print(f"Queueing time variation: " f"{coefficient_of_variation(best_fit_queueing_time):.2f}%")

def percentage_change(old, new):

    return ((new - old) / old) * 100


old_means = {"Makespan": np.mean(first_fit_makespan), 
             "Average Job Time": np.mean(first_fit_average_job_time), "Throughput": np.mean(first_fit_throughput),
             "Queueing Time": np.mean(first_fit_queueing_time)
}

new_means = {"Makespan": np.mean(best_fit_makespan),
             "Average Job Time": np.mean(best_fit_average_job_time),
             "Throughput": np.mean(best_fit_throughput),
             "Queueing Time": np.mean(best_fit_queueing_time)
}

print("\n========== FIRST FIT vs BEST FIT ==========")

print( f"{'Metric':<25}" f"{'First-Fit':>15}" f"{'Upgraded':>15}" f"{'Change':>15}")

print("-" * 70)

for metric in old_means:

    old = old_means[metric]
    new = new_means[metric]

    change = percentage_change(old, new)

    print(f"{metric:<25}" f"{old:>15.2f}" f"{new:>15.2f}" f"{change:>14.2f}%"
    )

# Graph 1 (Makespan comparison)

plt.figure()

plt.plot(
    runs,
    first_fit_makespan,
    marker="o",
    label="First Fit"
)

plt.plot(
    runs,
    best_fit_makespan,
    marker="o",
    label="Best Fit"
)

plt.axhline(
    np.mean(first_fit_makespan),
    linestyle="--",
    label=f"Mean = {np.mean(first_fit_makespan):.1f} ms"
)

plt.axhline(
    np.mean(first_fit_makespan),
    linestyle="--",
    label=f"Best Fit Mean = {np.mean(best_fit_makespan):.1f} ms"
)

plt.xlabel("Run")
plt.ylabel("Makespan (ms)")
plt.title("First Fit vs Best Fit - Makespan Comparison Across 10 Runs")
plt.xticks(runs)
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig(RESULTS_DIR / "makespan_comparison.svg", bbox_inches="tight")
plt.close()

# Graph 2 (Throughput)

plt.figure()

plt.plot(
    runs,
    first_fit_throughput,
    marker="o",
    label="First Fit"
)

plt.plot(
    runs,
    best_fit_throughput,
    marker="o",
    label="Best Fit Scheduler"
)

plt.axhline(
    np.mean(first_fit_throughput),
    linestyle="--",
    label=f"Mean = {np.mean(first_fit_throughput):.2f} jobs/s"
)

plt.axhline(
    np.mean(best_fit_throughput),
    linestyle="--",
    label=f"Best Fit Mean = {np.mean(best_fit_throughput):.2f} jobs/s"
)

plt.xlabel("Run")
plt.ylabel("Throughput (jobs/s)")
plt.title("First Fit vs Best Fit - Throughput Comparison Across 10 Runs")
plt.xticks(runs)
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig(RESULTS_DIR / "throughput_comparison.svg", bbox_inches="tight")
plt.close()

# Graph 3 (Worker Utilisation)

plt.figure()

plt.plot(
    runs,
    best_fit_cpu_utilisation,
    marker="o"
)

plt.axhline(
    np.mean(best_fit_cpu_utilisation),
    linestyle="--",
    label=f"Mean = {np.mean(best_fit_cpu_utilisation):.2f}%"
)

plt.xlabel("Run")
plt.ylabel("CPU Utilisation (%)")
plt.title("Best Fit Scheduler — CPU Utilisation")

plt.xticks(runs)
plt.legend()
plt.grid(True)
plt.tight_layout()

plt.savefig(
    RESULTS_DIR / "cpu_utilisation.svg",
    bbox_inches="tight"
)

plt.close()

plt.figure()

plt.plot(
    runs,
    best_fit_gpu_utilisation,
    marker="o"
)

plt.axhline(
    np.mean(best_fit_gpu_utilisation),
    linestyle="--",
    label=f"Mean = {np.mean(best_fit_gpu_utilisation):.2f}%"
)

plt.xlabel("Run")
plt.ylabel("GPU Utilisation (%)")
plt.title("Best Fit Scheduler — GPU Utilisation")

plt.xticks(runs)
plt.legend()
plt.grid(True)
plt.tight_layout()

plt.savefig(
    RESULTS_DIR / "gpu_utilisation.svg",
    bbox_inches="tight"
)

plt.close()

plt.figure()

plt.plot(
    runs,
    best_fit_memory_utilisation,
    marker="o"
)

plt.axhline(
    np.mean(best_fit_memory_utilisation),
    linestyle="--",
    label=f"Mean = {np.mean(best_fit_memory_utilisation):.2f}%"
)

plt.xlabel("Run")
plt.ylabel("Memory Utilisation (%)")
plt.title("Best Fit Scheduler — Memory Utilisation")

plt.xticks(runs)
plt.legend()
plt.grid(True)
plt.tight_layout()

plt.savefig(
    RESULTS_DIR / "memory_utilisation.svg",
    bbox_inches="tight"
)

plt.close()

# Graph 4 (Queueing Time)

plt.figure()

plt.plot(
    runs,
    first_fit_queueing_time,
    marker="o",
    label="First Fit"
)

plt.plot(
    runs,
    best_fit_queueing_time,
    marker="o",
    label="Best Fit Scheduler"
)

plt.axhline(
    np.mean(first_fit_queueing_time),
    linestyle="--",
    label=f"Mean = {np.mean(first_fit_queueing_time):.1f} ms"
)

plt.axhline(
    np.mean(best_fit_queueing_time),
    linestyle="--",
    label=f"Best Fit Mean = {np.mean(best_fit_queueing_time):.1f} ms"
)

plt.xlabel("Run")
plt.ylabel("Average Queueing Time (ms)")
plt.title("First Fit vs Best Fit - Queueing Time Comparison Across 10 Runs")
plt.xticks(runs)
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig(RESULTS_DIR / "queueing_time_comparison.svg", bbox_inches="tight")
plt.close()

# Graph 5 (Metrics Normalised)
# Normalise metrics as this helps to compare them and highlights useful information
# about their relativity

metrics = {
    "Makespan": best_fit_makespan,
    "Avg Job Time": best_fit_average_job_time,
    "Throughput": best_fit_throughput,
    "CPU Utilisation": best_fit_cpu_utilisation,
    "GPU Utilisation": best_fit_gpu_utilisation,
    "Memory Utilisation": best_fit_memory_utilisation,
    "Queue Time": best_fit_queueing_time
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
plt.title("Best Fit Scheduler — Relative Metric Variation")
plt.xticks(runs)
plt.legend()

plt.grid(True)
plt.tight_layout()
plt.savefig(RESULTS_DIR / "best_fit_relative_metrics.svg", bbox_inches="tight")
plt.close()

# Graph 6 (Average Job Time)

plt.figure()

plt.plot(
    runs,
    first_fit_average_job_time,
    marker="o",
    label="First-Fit"
)

plt.plot(
    runs,
    best_fit_average_job_time,
    marker="o",
    label="Best Fit Scheduler"
)

plt.axhline(
    np.mean(first_fit_average_job_time),
    linestyle="--",
    label=f"Mean = {np.mean(first_fit_average_job_time):.2f} ms"
)

plt.axhline(
    np.mean(best_fit_average_job_time),
    linestyle="--",
    label=f"Best Fit Mean = {np.mean(best_fit_average_job_time):.2f} ms"
)

plt.xlabel("Run")
plt.ylabel("Average Job Execution Time (ms)")
plt.title("First Fit vs Best Fit - Average Job Execution Time Comparison Across 10 Runs")
plt.xticks(runs)
plt.legend()
plt.grid(True)
plt.tight_layout()

plt.savefig(RESULTS_DIR / "average_job_time_comparison.svg", bbox_inches="tight")
plt.close()