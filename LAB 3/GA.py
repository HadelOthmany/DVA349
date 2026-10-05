import math
import numpy as np
import random
import matplotlib.pyplot as plt

MAX_FITNESS = 250000


# Load TSP file

def load_file(filepath):
    coordinates = {}
    with open(filepath) as file:
        for line in file:
            line = line.strip().split()
            if len(line) == 3 and line[0].isdigit():
                location_id = int(line[0])
                x, y = float(line[1]), float(line[2])
                coordinates[location_id] = (x, y)
    return coordinates


# Distance calculations
def calculate_distance(a, b):
    return math.hypot(a[0] - b[0], a[1] - b[1])


def calculate_total_distance(route, coordinates):
    total_distance = 0
    for i in range(len(route) - 1):
        total_distance += calculate_distance(
            coordinates[route[i]],
            coordinates[route[i + 1]]
        )
    return total_distance

def route_distance(path, dist_matrix):
    # path is 0-based indices, includes all cities exactly once, starts at 0
    total = 0.0
    for i in range(len(path) - 1):
        total += dist_matrix[path[i]][path[i + 1]]
    total += dist_matrix[path[-1]][path[0]]  # return to start
    return total


# Fitness (must increase)
def calculate_fitness(route, coordinates):
    distance = calculate_total_distance(route, coordinates)
    return 1 / (1 + distance)


# Create initial population
def create_population(locations, size):
    population = []
    cities = list(locations)
    cities.remove(1)

    while len(population) < size:
        shuffled = cities[:]
        random.shuffle(shuffled)
        route = [1] + shuffled + [1]
        population.append(route)

    return population


# Tournament selection
def select_parent(population, fitnesses, tournament_size=3):
    participants = random.sample(range(len(population)), tournament_size)
    best = max(participants, key=lambda i: fitnesses[i])
    return population[best]



# Order Crossover

def crossover(parent1, parent2):
    size = len(parent1)
    start, end = sorted(random.sample(range(1, size - 1), 2))

    child = [None] * size
    child[0] = 1
    child[-1] = 1

    # Copy slice from parent1
    child[start:end] = parent1[start:end]

    # Fill remaining from parent2
    fill_values = [c for c in parent2[1:-1] if c not in child]
    fill_positions = [i for i in range(1, size - 1) if child[i] is None]

    for pos, val in zip(fill_positions, fill_values):
        child[pos] = val

    return child


# Inversion mutation 
def mutate(route, mutation_rate=0.02):
    if random.random() < mutation_rate:
        i, j = sorted(random.sample(range(1, len(route) - 1), 2))
        route[i:j] = reversed(route[i:j])


# Main GA
def genetic_algorithm(coordinates, pop_size=200, mutation_rate=0.2):

    population = create_population(coordinates.keys(), pop_size)

    fitness_evaluations = 0
    best_route = None
    best_distance = float("inf")
    history = []

    elitism = 2

    while fitness_evaluations < MAX_FITNESS:

        # Evaluate population
        distances = []
        fitnesses = []

        for route in population:
            distance = calculate_total_distance(route, coordinates)
            fitness = 1 / (1 + distance)

            distances.append(distance)
            fitnesses.append(fitness)

            fitness_evaluations += 1
            if fitness_evaluations >= MAX_FITNESS:
                break

        # Track best solution
        gen_best_index = min(range(len(distances)), key=lambda i: distances[i])

        if distances[gen_best_index] < best_distance:
            best_distance = distances[gen_best_index]
            best_route = population[gen_best_index][:]

        history.append(1 / (1 + best_distance))

        if fitness_evaluations >= MAX_FITNESS:
            break

        # Elitism
        elite_indices = sorted(range(len(distances)), key=lambda i: distances[i])[:elitism]
        new_population = [population[i][:] for i in elite_indices]

        # Generate offspring
        while len(new_population) < pop_size:
            parent1 = select_parent(population, fitnesses)
            parent2 = select_parent(population, fitnesses)

            child1 = crossover(parent1, parent2)
            child2 = crossover(parent2, parent1)

            mutate(child1, mutation_rate)
            mutate(child2, mutation_rate)

            new_population.append(child1)
            if len(new_population) < pop_size:
                new_population.append(child2)

        population = new_population

    return best_route, best_distance, history

# Main ACO
def ACO(coordinates, num_ants=50, num_iterations=120,
        alpha=1.0, beta=5.0, evaporation_rate=0.5, Q=100):

    cities = list(coordinates.keys())
    n = len(cities)

    # Distance matrix using city IDs
    dist = {i: {} for i in cities}
    for i in cities:
        for j in cities:
            dist[i][j] = calculate_distance(coordinates[i], coordinates[j])

    # Pheromone matrix
    pheromone = {i: {j: 1.0 for j in cities} for i in cities}

    best_route = None
    best_distance = float("inf")
    history = []

    for iteration in range(num_iterations):

        all_routes = []
        all_distances = []

        for ant in range(num_ants):

            route = [1]
            visited = set(route)

            while len(route) < n:

                current = route[-1]
                probabilities = []
                candidates = []

                for city in cities:
                    if city not in visited:

                        tau = pheromone[current][city] ** alpha
                        eta = (1 / dist[current][city]) ** beta

                        probabilities.append(tau * eta)
                        candidates.append(city)

                prob_sum = sum(probabilities)

                if prob_sum == 0:
                    next_city = random.choice(candidates)
                else:
                    probabilities = [p / prob_sum for p in probabilities]
                    next_city = random.choices(candidates, weights=probabilities)[0]

                route.append(next_city)
                visited.add(next_city)

            route.append(1)

            distance = calculate_total_distance(route, coordinates)

            all_routes.append(route)
            all_distances.append(distance)

            if distance < best_distance:
                best_distance = distance
                best_route = route[:]

        # Evaporation
        for i in cities:
            for j in cities:
                pheromone[i][j] *= (1 - evaporation_rate)

        # Deposit pheromone
        for route, distance in zip(all_routes, all_distances):

            deposit = Q / distance

            for i in range(len(route) - 1):
                a = route[i]
                b = route[i + 1]

                pheromone[a][b] += deposit
                pheromone[b][a] += deposit

        history.append(best_distance)

    return best_route, best_distance, history

# Run
if __name__ == "__main__":
    filepath = "Assignment 3 berlin52.tsp"
    coordinates = load_file(filepath)

    best_route, best_distance, history = genetic_algorithm(coordinates)
   
    best_route_aco, best_distance_aco, history_aco = ACO(coordinates)

    print(f"Best route found by GA: {best_route} with distance {best_distance:.2f}")
    print(f"Best route found by ACO: {best_route_aco} with distance {best_distance_aco:.2f}")

    plt.plot(history)
    plt.xlabel("Generation")
    plt.ylabel("Best Fitness")
    plt.title("GA Fitness Over Generations")
    plt.show()

    # Plot ACO history
    plt.plot(history_aco)
    plt.xlabel("Iteration")
    plt.ylabel("Best Distance")
    plt.title("ACO Best Distance Over Iterations")
    plt.show()