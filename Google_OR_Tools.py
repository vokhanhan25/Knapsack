from ortools.algorithms import pywrapknapsack_solver
import data
import time
import os
def main():
    
    inp = data.input_data()
    # solver = pywrapknapsack_solver.KnapsackSolver(
        # pywrapknapsack_solver.KnapsackSolver.KNAPSACK_MULTIDIMENSION_SCIP_MIP_SOLVER,'0-1 knapsack problems')
    solver = pywrapknapsack_solver.KnapsackSolver(
        pywrapknapsack_solver.KnapsackSolver.
        KNAPSACK_MULTIDIMENSION_BRANCH_AND_BOUND_SOLVER, 'KnapsackExample')

    for file_name in range(0, len(inp)):
        
        with open("data/" + inp[file_name] + ".kp") as inp_file:
            read_file = inp_file.read().split('\n')
            
        number_items = (int)(read_file[1])
        capacities = [(int)(read_file[2])]
        values = []
        weights = [[]]
        print(file_name, number_items)
        for i in range(4, number_items + 4):
            x = read_file[i].split(" ")[0]
            y = read_file[i].split(" ")[1]
            values.append((int)(x))
            weights[0].append((int)(y))

        start_time = time.time()
        
        solver.Init(values, weights, capacities)
        solver.set_time_limit(300)
        computed_value = solver.Solve()
        
        time_step = (time.time() - start_time)

        # packed_items = []
        # packed_weights = []
        total_weight = 0
        
        for i in range(len(values)):
                if solver.BestSolutionContains(i):
                    # packed_items.append(i)
                    # packed_weights.append(weights[0][i])
                    total_weight += weights[0][i]
        
        with open("output/OR-Tools_Branch_Bound/" + inp[file_name] + ".txt", "w") as output_file:
        # with open("output/OR-Tools_SCIP_MIP_Solver/" + inp[file_name] + ".txt", "w") as output_file:
            output_file.write('File name: ' + inp[file_name] + "\n")
            output_file.write ('Number of items: ' + str(number_items) + "\n")
            output_file.write('Total value: ' + str(computed_value) + "\n")
            
            output_file.write('Total weight: ' +  str(total_weight) + "\n")
            output_file.write('Capacity: ' + str(capacities[0]) + "\n")
            output_file.write('Runtime: ' + str(time_step) + " seconds\n")
            # print('Packed items:', packed_items)
            # print('Packed_weights:', packed_weights)
            # print('/--------------------------/\n\n')
    
if __name__ == '__main__':
    main()