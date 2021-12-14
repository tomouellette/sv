from sv_evolution import sv_evolution
import numpy as np
import pandas as pd

# Set parameters
population_size = 1000
generations = 100
mutation_rate = 1e-8
sv_length_distribution = 'uniform'

# Run simulation
mut_ids, sv_lengths, frequency = sv_evolution(
    population_size, 
    generations, 
    mutation_rate,
    sv_length_distribution
)

# Plot SV frequency distribution
import matplotlib.pyplot as plt
import seaborn as sns
sns.set_theme(style='ticks')
fig, ax = plt.subplots(1,2)
sns.histplot(frequency, bins = 100, kde=True, edgecolor = None, ax=ax[0])
ax[0].set_xlabel('SV fraction')

ax[1].scatter(sv_lengths, frequency)
ax[1].set_xlabel('SV length')
ax[1].set_ylabel('SV frequency')

fig.tight_layout()
plt.show()
