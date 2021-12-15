from sv_evolution import sv_evolution
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

class Simulator:
    def __init__(self, params: dict):
        self.population_size = params['population_size']
        self.generations = params['generations']
        self.mutation_rate = params['mutation_rate']
        self.sv_length_distribution = params['sv_length_distribution']
        
    def run(self):
        sim = sv_evolution(self.population_size, self.generations, self.mutation_rate, self.sv_length_distribution)
        self.mutation_ids, self.sv_lengths, self.frequency = sim
    
    def plot(self, save = None):
        sns.set_theme(style='ticks')
        fig, ax = plt.subplots(1,2)
        sns.histplot(self.frequency, bins = 100, kde=True, edgecolor = None, ax=ax[0])
        ax[0].set_xlabel('SV fraction')        
        
        sns.scatterplot(self.sv_lengths, self.frequency, ax=ax[1])
        ax[1].set_xlabel('SV length')
        ax[1].set_ylabel('SV frequency')
        
        fig.suptitle(f'Params:\n $N$: {self.population_size}, $gen$: {self.generations}, $\mu$ (genome/div): {self.mutation_rate}, SV length distribution: {self.sv_length_distribution}', size=10)
        fig.subplots_adjust(bottom=0.2)
        #fig.tight_layout()
        
        if save == None:    
            plt.show()
        else:
            plt.savefig(save)
            
    def data(self):
        return self.mutation_ids, self.sv_lengths, self.frequency
