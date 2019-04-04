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

#define MAXIMUM_TASKS_ALLOWED 31

#define MAX(a, b) (a <= b ? b : a)
#define MIN(a, b) (a <= b ? a : b)

#define SET_BIT(num, n) ((1 << n) | num)
#define TEST_BIT(num, n) ((1 << n) & num)

#define GET_TIME(ini, fim) ((fim - ini) / (float) CLOCKS_PER_SEC)

struct Task {
	Task() {
		id = d1 = d2 = 0;
	}

	Task(char _id, int _d1, int _d2) {
		id = _id;
		d1 = _d1;
		d2 = _d2;
	}

	char id;
	int d1, d2;
};

struct Node {
	Node() {
		tasks = sumF2 = f1 = f2 = 0;
	}

	Node(int _f1, int _f2, int _sum) {
		f1 = _f1;
		f2 = _f2;
		tasks = 0;
		sumF2 = _sum;
	}

	bool operator <(const Node &other) const {
		return sumF2 > other.sumF2 || (sumF2 == other.sumF2 && f2 > other.f2);
	}

	int f1;
	int f2;
	int sumF2;
	int tasks;
	char orderTasks[MAXIMUM_TASKS_ALLOWED];
};

class FlowshopBB {
private:
	Node bestDual;
	float timeFoundBestDual = 0;

	Node bestPrimal;
	float timeFoundBestPrimal = 0;

	std::vector<Task> tasks;
	clock_t initialTime = 0;
	unsigned long numExploredNodes = 0;
	unsigned long limitExploredNodes = 0;
	float maximumAllowedTime = 0;
	std::vector<Task> tasksSortedD1;
	std::vector<Task> tasksSortedD2;
	std::map<int, std::stack<Node>> *activeNodes; /* map<estimated F Value, Node> */
	std::unordered_map<int, std::pair<int, int>> *dominance; /* map<active tasks from binary to decimal representation, <f2_tr, sumF2_tr of tasks actives>> */

public:
	void solve();
	~FlowshopBB();

	Node getBestDual();
	Node getBestPrimal();
	float getElapsedTime();
	unsigned long getExploredNodes();
	float getTimeFoundBestDual();
	float getTimeFoundBestPrimal();
	void addTask(int d1, int d2);
	FlowshopBB(unsigned long _limitExploredNodes, int _maximumAllowedTime, clock_t _initialTime);
};

#endif //FLOWSHOP_BB_H
