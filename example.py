from sv import Simulator

params = {
    'population_size': 1000,
    'generations': 100,
    'mutation_rate': 0.001,
    'sv_length_distribution': 'uniform'
}

simulator = Simulator(params)

simulator.run()
simulator.plot()
mutation_ids, sv_lengths, frequency = simulator.data()