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

#include "FlowshopBB.h"

FlowshopBB::~FlowshopBB() {}

FlowshopBB::FlowshopBB(int _limitExploredNodes, int _maximumAllowedTime) {
	initialTime = clock(); /* initial execution time */

	exploredNodes = 0;
	limitExploredNodes = _limitExploredNodes;
	maximumAllowedTime = (float) _maximumAllowedTime;

	// printf("limit maxtime: %d %f\n", limitExploredNodes, maximumAllowedTime);
}

void FlowshopBB::addTask(int d1, int d2) {
	tasks.emplace_back(tasks.size(), d1, d2);
}

void FlowshopBB::solve() {
	std::sort(tasks.begin(), tasks.end());
	
	// printf("Sorted array!\n");
	//
	// for (auto &t : tasks) {
	// 	printf("Task %d: %d %d\n", t.id, t.d1, t.d2);
	// }
}
