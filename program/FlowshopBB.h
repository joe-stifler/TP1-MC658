/**
 * University: UNICAMP
 * Discipline: MC658
 * Professor: Cid C. de Souza
 * PED: Natanael Ramos
 * Data of creation: March 30, 2019
 * Author1 (RA 176665): Jose Ribeiro Neto <j176665@dac.unicamp.br>
 * Author2 (RA XXXXXX):  <@>
 *
 * File: FlowshopBB.h
 **/

#ifndef FLOWSHOP_BB_H
#define FLOWSHOP_BB_H

#include <map>
#include <queue>
#include <ctime>
#include <cstdio>
#include <string>
#include <vector>
#include <bitset>
#include <climits>
#include <algorithm>

#define MAXIMUM_TASKS_ALLOWED 62
#define getTime(ini, fim) ((fim - ini) / (float) CLOCKS_PER_SEC)

struct Task {
	Task() {
		id = d1 = d2 = 0;
	}

	Task(int _id, int _d1, int _d2) {
		id = _id;
		d1 = _d1;
		d2 = _d2;
	}

	int id;
	int d1, d2;
};

struct Node {
	Node() {
		tasks.reset();
		sumF2 = f1 = f2 = 0;
	}

	Node(int _f1, int _f2, int _sum) {
		f1 = _f1;
		f2 = _f2;
		sumF2 = _sum;
	}

	int f1; /**/
	int f2; /**/
	int sumF2;
	std::bitset<MAXIMUM_TASKS_ALLOWED> tasks; /**/
};


class FlowshopBB {
private:
	Node bestNode;
	clock_t initialTime;
	int numExploredNodes;
	int limitExploredNodes;
	float maximumAllowedTime;
	std::vector<Task> tasks;
	std::vector<Task> tasksSortedD1;
	std::vector<Task> tasksSortedD2;
	std::map<int, std::queue<Node>> *activeNodes;

public:
	void solve();
	~FlowshopBB();

	Node getBestNode();
	int getExploredNodes();
	void addTask(int d1, int d2);
	FlowshopBB(int _limitExploredNodes, int _maximumAllowedTime);
};

#endif //FLOWSHOP_BB_H
