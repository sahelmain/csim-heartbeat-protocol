import matplotlib.pyplot as plt
import numpy as np

# Data from simulation results
loss_probabilities = [0.1, 0.2, 0.3, 0.4, 0.5]
success_percentages = [54.37, 51.04, 41.86, 36.28, 31.32]
failure_percentages = [45.47, 48.61, 57.95, 63.72, 68.33]

# Create figure with two subplots
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))

# Graph 1: Percentage of Successful Transmissions
ax1.plot(loss_probabilities, success_percentages, 'bo-', linewidth=2, markersize=8, color='#2E8B57')
ax1.set_xlabel('Packet Loss Probability', fontsize=12, fontweight='bold')
ax1.set_ylabel('Success Rate (%)', fontsize=12, fontweight='bold')
ax1.set_title('Percentage of Successful Transmissions vs Packet Loss Probability', 
              fontsize=14, fontweight='bold', pad=20)
ax1.grid(True, alpha=0.3)
ax1.set_xlim(0.05, 0.55)
ax1.set_ylim(25, 60)

# Add data labels
for i, (x, y) in enumerate(zip(loss_probabilities, success_percentages)):
    ax1.annotate(f'{y:.1f}%', (x, y), textcoords="offset points", 
                xytext=(0,10), ha='center', fontsize=10, fontweight='bold')

# Graph 2: Percentage of Failed Transmissions  
ax2.plot(loss_probabilities, failure_percentages, 'ro-', linewidth=2, markersize=8, color='#DC143C')
ax2.set_xlabel('Packet Loss Probability', fontsize=12, fontweight='bold')
ax2.set_ylabel('Failure Rate (%)', fontsize=12, fontweight='bold')
ax2.set_title('Percentage of Failed Transmissions vs Packet Loss Probability', 
              fontsize=14, fontweight='bold', pad=20)
ax2.grid(True, alpha=0.3)
ax2.set_xlim(0.05, 0.55)
ax2.set_ylim(40, 75)

# Add data labels
for i, (x, y) in enumerate(zip(loss_probabilities, failure_percentages)):
    ax2.annotate(f'{y:.1f}%', (x, y), textcoords="offset points", 
                xytext=(0,10), ha='center', fontsize=10, fontweight='bold')

# Overall title
fig.suptitle('Heartbeat Protocol Performance Analysis\nProject #2: Azzam, Sahel', 
             fontsize=16, fontweight='bold', y=0.98)

# Adjust layout
plt.tight_layout()
plt.subplots_adjust(top=0.85)

# Save the graph
plt.savefig('heartbeat_protocol_graphs.png', dpi=300, bbox_inches='tight')
plt.show()

# Print summary statistics
print("=== SIMULATION SUMMARY ===")
print("Loss Probability | Success Rate | Failure Rate")
print("-" * 45)
for i in range(len(loss_probabilities)):
    print(f"     {loss_probabilities[i]:.1f}        |    {success_percentages[i]:.2f}%   |   {failure_percentages[i]:.2f}%")

print(f"\nTrend Analysis:")
print(f"• Success rate decreases from {success_percentages[0]:.1f}% to {success_percentages[-1]:.1f}%")
print(f"• Failure rate increases from {failure_percentages[0]:.1f}% to {failure_percentages[-1]:.1f}%")
print(f"• Performance degrades monotonically with increasing packet loss") 