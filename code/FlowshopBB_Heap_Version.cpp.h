/**
 * University: UNICAMP
 * Discipline: MC658
 * Professor: Cid C. de Souza
 * PED: Natanael Ramos
 * Author1 (RA 176665): Jose Ribeiro Neto <j176665@dac.unicamp.br>
 * Author2 (RA XXXXXX):  <@>
 *
 * File: FlowshopBB.h
 * Data of creation: March 30, 2019
 **/

#ifndef FLOWSHOP_BB_H
#define FLOWSHOP_BB_H

#include <map>
#include <stack>
#include <queue>
#include <ctime>
#include <cstdio>
#include <string>
#include <vector>
#include <bitset>
#include <climits>
#include <algorithm>
#include <unordered_map>

#define MAXIMUM_TASKS_ALLOWED 31 /* This assumption is necessary, since the program uses a bitset mask with exactly 32 bits */

#define MAX(a, b) (a <= b ? b : a)
#define MIN(a, b) (a <= b ? a : b)

#define SET_BIT(num, n) ((1 << n) | num) /* Sets the n-th bit of num */
#define TEST_BIT(num, n) ((1 << n) & num) /* Tests the n-th bit of num */

#define GET_TIME(ini, fim) ((fim - ini) / (float) CLOCKS_PER_SEC) /* Gets the elapsed time from fim-ini */

struct Task {
	Task() {
		id = d1 = d2 = 0;
	}

	Task(char _id, int _d1, int _d2) {
		id = _id;
		d1 = _d1;
		d2 = _d2;
	}

	char id; /* Task identification */
	int d1; /* Duration of this task on machine 1 */
	int d2; /* Duration of this task on machine 2 */
};

struct Node {
	Node() {
		estimate = tasks = sumF2 = f1 = f2 = 0;
	}

	Node(int _f1, int _f2, int _sum) {
		f1 = _f1;
		f2 = _f2;
		tasks = 0;
		sumF2 = _sum;
		estimate = 0;
	}

	bool operator <(const Node &other) const {
		return estimate > other.estimate;
		// return sumF2 > other.sumF2;
	}

	int estimate;
	int f1; /* Finalization time on machine 1 of the last scheduled task (according to 'tasks' variable) */
	int f2; /* Finalization time on machine 2 of the last scheduled task (according to 'tasks' variable) */
	int sumF2; /* Summation of all finalization times (of the the previous scheduled tasks, according to 'tasks' variable) on machine 2 */
	int tasks; /* Variable representing the already scheduled tasks. When the i-th bit is set, then the i-th task is already scheduled */
	char orderTasks[MAXIMUM_TASKS_ALLOWED]; /* Vector representing the order in which the tasks were scheduled.
											   This vector is filled until the count_number_of_setted_bits(tasks) position. */
};

class FlowshopBB {
private:
	Node bestDual;
	float timeFoundBestDual = 0;

	Node bestPrimal;
	float timeFoundBestPrimal = 0;

	std::vector<Task> tasks;
	clock_t initialTime = 0;
	float maximumAllowedTime = 0;
	std::vector<Task> tasksSortedD1;
	std::vector<Task> tasksSortedD2;
	unsigned long numExploredNodes = 0;
	unsigned long limitExploredNodes = 0;
	std::priority_queue<Node> *activeNodes; /* map<estimated F Value, Node> */
	std::unordered_map<int, std::pair<int, int>> *dominance; /* map<active tasks from binary to decimal
																		representation, <f2_tr, sumF2_tr of tasks actives>> */

public:
	/**
	 * Solves the Flowshop problem using branch-and-bound, with a best bound strategy.
	 *
	 * This function implements the algorithm branch-and-bound for the Flowshop Problem,
	 * described in pages 444-448 from "Combinatorial Optimization" of Papadimitriou and
	 * Steiglitz, Prentice-Hall INC., 1982.
	 *
	 * In this implementation, three important strategies were applied:
	 *
	 * - For the branch-and-bound algorithm a best-bound strategy was used during the state space exploration.
	 *
	 * - In each node tree (beeing explored), beyond the two dual nodes computed, the algorithm also computes the two
	 * primal nodes related with the task order used during the dual nodes computation.
	 *
	 * -
	 **/
	void solve();

	/**
	 * Destructor. Deallocates allocated memory
	 **/
	~FlowshopBB();

	/**
	 * Returns the best dual node found in the solve() process
	 **/
	Node getBestDual();

	/**
	 * Returns the best primal node found during the solve() process
	 **/
	Node getBestPrimal();

	/**
	 * Returns the elapsed time since the first time measure (given by initialTime)
	 **/
	float getElapsedTime();

	/**
	 * Returns the total number of nodes that passed through 'activeNodes'
	 **/
	unsigned long getExploredNodes();

	/**
	 * Returns the time when the best dual node was found during the solve() process
	 **/
	float getTimeFoundBestDual();

	/**
	 * Returns the time when the best primal node was found during the solve() process
	 **/
	float getTimeFoundBestPrimal();

	/**
	 * Adds a new task to tasks, tasksSortedD1 and tasksSortedD2
	 *
	 * Params:
	 *     - d1 : duration time on machine 1 for the task to be added
	 *     - d2 : duration time on machine 2 for the task to be added
	 **/
	void addTask(int d1, int d2);

	/**
	 * Constructor. Initializes data structures, initial time of execution.
	 *
	 * Params:
	 *     - _limitExploredNodes : The maximum allowed number of nodes to pass through activeNodes
	 *     - _maximumAllowedTime : The maximum elapsed time of execution since the first time measure (given by _initialTime)
	 *     - _initialTime : The first time measure
	 **/
	FlowshopBB(unsigned long _limitExploredNodes, int _maximumAllowedTime, clock_t _initialTime);
};

#endif //FLOWSHOP_BB_H
