/**
 * University: UNICAMP
 * Discipline: MC658
 * Professor: Cid C. de Souza
 * PED: Natanael Ramos
 * Data of creation: March 30, 2019
 * Author (RA 176665): Jose Ribeiro Neto <j176665@dac.unicamp.br>
 * Author2 (RA XXXXXX):  <@>
 *
 * File: FlowshopBB.h
 **/

#ifndef FLOWSHOP_BB_H
#define FLOWSHOP_BB_H

#include <ctime>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>

#define getTime(ini, fim) ((fim - ini) / (float) CLOCKS_PER_SEC)

struct Task {
	Task(int _id, int _d1, int _d2) {
		id = _id;
		d1 = _d1;
		d2 = _d2;
	}

	bool operator <(const Task &other) const {
		return d1 < other.d1 || (d1 == other.d1 && d2 < other.d2);
	}

	int id;
	int d1, d2;
};


class FlowshopBB {
private:
	int exploredNodes;
	clock_t initialTime;
	int limitExploredNodes;
	std::vector<Task> tasks;
	float maximumAllowedTime;

public:
	void solve();
	~FlowshopBB();
	void addTask(int d1, int d2);
	FlowshopBB(int _limitExploredNodes, int _maximumAllowedTime);
};

#endif //FLOWSHOP_BB_H
