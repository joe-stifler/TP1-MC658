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

#include "FlowshopBB.h"

FlowshopBB::~FlowshopBB() {
	delete dominance;
	delete activeNodes;
}

FlowshopBB::FlowshopBB(unsigned long _limitExploredNodes, int _maximumAllowedTime, clock_t _initialTime) {
	initialTime = _initialTime; /* initial execution time */

	numExploredNodes = 0;
	limitExploredNodes = _limitExploredNodes;
	maximumAllowedTime = (float) _maximumAllowedTime;

	bestDual = Node(0, 0, INT_MAX);
	bestPrimal = Node(0, 0, INT_MAX);

	activeNodes = new std::map<int, std::queue<Node>>();
	dominance = new std::unordered_map<int, std::pair<int, int>>();
}

void FlowshopBB::addTask(int d1, int d2) {
	tasks.emplace_back(tasks.size(), d1, d2);
	tasksSortedD1.emplace_back(tasksSortedD1.size(), d1, d2);
	tasksSortedD2.emplace_back(tasksSortedD2.size(), d1, d2);
}

void FlowshopBB::solve() {
	char numTasks = tasksSortedD1.size();

	/* Sorts tasks by ascending order using d1 as key */
	std::sort(tasksSortedD1.begin(), tasksSortedD1.end(), [] (const Task& t1, const Task& t2) -> bool {
       return t1.d1 < t2.d1 || (t1.d1 == t2.d1 && t1.d2 < t2.d2);
    });

	/* Sorts tasks by ascending order using d2 as key */
	std::sort(tasksSortedD2.begin(), tasksSortedD2.end(), [] (const Task& t1, const Task& t2) -> bool {
       return t1.d2 < t2.d2 || (t1.d2 == t2.d2 && t1.d1 < t2.d1);
    });

	activeNodes->insert(std::make_pair(0, std::queue<Node>({Node()})));

	int f2;
	int minD1;
	char r, i;
	int s1, s2;
	char k1, k2;
	bool minDFound;
	int estimatedF;
	Node nodeR, currentNode;
	char remainTaskstoSchedule;
	std::map<int, std::queue<Node>>::iterator topNode;
	std::unordered_map<int, std::pair<int, int>>::iterator refDominance;
	Task *taskR, *task1, *task2, *endTask1 = tasksSortedD1.data() + numTasks;

	/* Explore the tree */
	while(limitExploredNodes > numExploredNodes && activeNodes->size() > 0 && maximumAllowedTime > GET_TIME(initialTime, clock())) {
		topNode = activeNodes->begin();

		currentNode = topNode->second.front();
		topNode->second.pop();

		if (topNode->second.size() == 0) activeNodes->erase(topNode);

		/* Count the number of already scheduled tasks + the one to be scheduled */
		r = 1 + __builtin_popcount(currentNode.tasks);
		remainTaskstoSchedule = numTasks - r;

		refDominance = dominance->find(currentNode.tasks);

		/* applies dominance relation */
		if (refDominance != dominance->end()) {
			if (refDominance->second.first <= currentNode.f2 && refDominance->second.second <= currentNode.sumF2) continue;
			else {
				refDominance->second.first = currentNode.f2;
				refDominance->second.second = currentNode.sumF2;
			}
		} else dominance->insert(std::make_pair(currentNode.tasks, std::make_pair(currentNode.f2, currentNode.sumF2)));

		/* Branch step (Expand all currentNode's children) */
		for (i = 0, taskR = tasks.data(); i < numTasks; ++i, ++taskR) {
			numExploredNodes += (unsigned long) numTasks - r;

			if (limitExploredNodes < numExploredNodes) {
				numExploredNodes -= (unsigned long) numTasks - r;
				return;
			}

			/* Verifies if task i should be scheduled */
			if ((currentNode.tasks & (1 << i)) == 0) {
				minDFound = s1 = s2 = f2 = minD1 = 0;

				nodeR = currentNode;
				nodeR.tasks |= 1 << i; /* Marks task i as scheduled */
				nodeR.f1 += taskR->d1;
				nodeR.f2 = MAX(nodeR.f1, nodeR.f2) + taskR->d2;
				nodeR.sumF2 += nodeR.f2;
				// nodeR.order.push_back(char(i));

				/* verifies if it is a leaf node */
				if (r == numTasks) {
					/* verifies if a primal limitant was found */
					if (bestPrimal.sumF2 > nodeR.sumF2) {
						bestPrimal = nodeR;
						timeFoundBestPrimal = GET_TIME(initialTime, clock());
					}
				} else {
					task1 = tasksSortedD1.data();
					task2 = tasksSortedD2.data();

					/* found all tasks not scheduled yet */
					for (k1 = k2 = remainTaskstoSchedule; task1 < endTask1; ++task2, ++task1) {
						/* */
						if ((nodeR.tasks & (1 << task1->id)) == 0) {
							if (minDFound == false) {
								minD1 = task1->d1;
								minDFound = true;
							}

							s1 += k1 * task1->d1 + task1->d2;
							--k1;
						}

						if ((nodeR.tasks & (1 << task2->id)) == 0) {
							s2 += k2 * task2->d2;
							--k2;
						}
					}

					s1 += remainTaskstoSchedule * nodeR.f1;
					s2 += remainTaskstoSchedule * MAX(nodeR.f2, nodeR.f1 + minD1);

					estimatedF = nodeR.sumF2 + MAX(s1, s2);

					if (bestDual.sumF2 > estimatedF) {
						bestDual.sumF2 = estimatedF;
						timeFoundBestDual = GET_TIME(initialTime, clock());
					}

					/* Bound step (via primal limitant) */
					if (bestPrimal.sumF2 > estimatedF) {
						topNode = activeNodes->find(estimatedF);
						if (topNode == activeNodes->end())
							activeNodes->insert(std::make_pair(estimatedF, std::queue<Node>({nodeR})));
						else topNode->second.push(nodeR);
					}
				}
			}
		}
	}

	printf("Size map: %d\n", (int) activeNodes->size());
}

unsigned long FlowshopBB::getExploredNodes() {
	return numExploredNodes;
}

Node FlowshopBB::getBestDual() {
	return bestDual;
}

Node FlowshopBB::getBestPrimal() {
	return bestPrimal;
}

float FlowshopBB::getElapsedTime() {
	return GET_TIME(initialTime, clock());
}

float FlowshopBB::getTimeFoundBestDual() {
	return timeFoundBestDual;
}

float FlowshopBB::getTimeFoundBestPrimal() {
	return timeFoundBestPrimal;
}
