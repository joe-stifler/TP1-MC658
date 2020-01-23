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

FlowshopBB::FlowshopBB(unsigned long _limitExploredNodes, int _maximumAllowedTime, clock_t _initialTime) {
	initialTime = _initialTime; /* initial execution time */

	numExploredNodes = 0;
	limitExploredNodes = _limitExploredNodes;
	maximumAllowedTime = (float) _maximumAllowedTime;

	bestDual = Node(0, 0, INT_MAX);
	bestPrimal = Node(0, 0, INT_MAX);

	activeNodes = new std::priority_queue<Node>();
	dominance = new std::unordered_map<int, std::pair<int, int>>();
}

void FlowshopBB::addTask(int d1, int d2) {
	tasks.emplace_back(tasks.size(), d1, d2);
	tasksSortedD1.emplace_back(tasksSortedD1.size(), d1, d2);
	tasksSortedD2.emplace_back(tasksSortedD2.size(), d1, d2);
}

void FlowshopBB::solve() {
	int f2;
	int minD1;
	int s1, s2;
	char k1, k2;
	char rLast, i;
	bool minDFound;
	int estimatedSumF2;
	Node nodeR, currentNode;
	char remainTaskstoSchedule;
	Node auxPrimal1, auxPrimal2;
	char numTasks = (char) tasksSortedD1.size();
	std::unordered_map<int, std::pair<int, int>>::iterator refDominance;
	Task *taskR, *task1, *task2, *endTask1 = tasksSortedD1.data() + numTasks;

	// activeNodes->clear();
	(*activeNodes).push(Node());

	/* Initializes primal and dual to infinit */
	bestDual = Node(0, 0, INT_MAX);
	bestPrimal = Node(0, 0, INT_MAX);

	/* Sorts tasks by ascending order using d1 as key */
	std::sort(tasksSortedD1.begin(), tasksSortedD1.end(), [] (const Task& t1, const Task& t2) -> bool {
	   return t1.d1 < t2.d1 || (t1.d1 == t2.d1 && t1.d2 < t2.d2);
	});

	/* Sorts tasks by ascending order using d2 as key */
	std::sort(tasksSortedD2.begin(), tasksSortedD2.end(), [] (const Task& t1, const Task& t2) -> bool {
	   return t1.d2 < t2.d2 || (t1.d2 == t2.d2 && t1.d1 < t2.d1);
	});

	/* Explores the tree while the limit of explored nodes was not reached, while the elapsed
	 * time is lesser then the maximum allowed time and while there is active nodes. */
	while(limitExploredNodes > numExploredNodes && activeNodes->size() > 0 && maximumAllowedTime > GET_TIME(initialTime, clock())) {
		currentNode = activeNodes->top(); /* Gets the top node of the stack */
		activeNodes->pop(); /* Removes the top node from the stack */

		/* Verifies if the estimatedSumF2 (topNode->first) is better
		 * than the bestPrimal cost. In case of not, there is no meaning
		 * in exploring currentNode (or even the other nodes inside the stack),
		 * since the best we can get is estimatedSumF2 that it is worst than what
		 * we have with bestPrimal.sumF2. */
		if (bestPrimal.sumF2 <= currentNode.estimate) {
			/* Removes the whole stack associated with estimatedSumF2, and go to the next exploration */
			// activeNodes->erase(topNode);
			continue;
		}
		// else if (topNode->second.size() == 0) activeNodes->erase(topNode); /* Removes the empty stack from the tree */

		/* Counts the number of already scheduled tasks */
		rLast = __builtin_popcount(currentNode.tasks);

		/* Counts the number of tasks to be scheduled */
		remainTaskstoSchedule = numTasks - rLast - 1;

        /* The currentNode.tasks represents the tasks already
		 * scheduled (if i-th task is scheduled, then i-th bit
		 * of currentNode.tasks is set). But also, currentNode.tasks
		 * represents a unique integer number that can be used
		 * to apply dominance relation. The following line verifies
		 * the presence of this number in the dominance tree. */
		refDominance = dominance->find(currentNode.tasks);

		/* Applies dominance relation, in case the number currentNode.tasks was found in the dominance tree */
		if (refDominance != dominance->end()) {
			/* Verifies if currentNode is dominated by another node having the same .tasks value as currentNode.tasks. */
			if (refDominance->second.first <= currentNode.f2 && refDominance->second.second <= currentNode.sumF2) continue;
			else {
				/* In case it is not dominated, then substitute the values of
					currentNode.f2 and currentNode.sumF2 in the dominance tree node */

				/* Other useful strategies. */
				// if (refDominance->second.first  > currentNode.f2) {
				// refDominance->second.first = currentNode.f2;
				// refDominance->second.second = currentNode.sumF2;
				//
				// }

				/* Other useful strategies. The strategy below is terrible! Do not uncomment. */
				// if (refDominance->second.second  > currentNode.sumF2) {
				// refDominance->second.first = currentNode.f2;
				// refDominance->second.second = currentNode.sumF2;
				//
				// }

				refDominance->second.first = currentNode.f2;
				refDominance->second.second = currentNode.sumF2;
			}
		} else /* Inserts the number currentNode.tasks as a new node in the dominance tree, in case it was not found */
			dominance->insert(std::make_pair(currentNode.tasks, std::make_pair(currentNode.f2, currentNode.sumF2)));

		/* Branch step (Expand all possible currentNode's children) */
		for (i = 0, taskR = tasks.data(); i < numTasks; ++i, ++taskR) {
			/* Verifies if task i should be scheduled */
			if ((currentNode.tasks & (1 << i)) == 0) {
				minDFound = s1 = s2 = f2 = minD1 = 0;

				nodeR = currentNode; /* Copies all the previous nodes configurations */
				nodeR.tasks |= (1 << i); /* Marks task i as scheduled */
				nodeR.f1 += taskR->d1; /* Calculates finish time on machine 1 of i-th task */
				nodeR.f2 = MAX(nodeR.f1, nodeR.f2) + taskR->d2; /* calculates finish time on machine 2 of i-th task */
				nodeR.sumF2 += nodeR.f2; /* Calculates accumulated finish time on machine 2 */
				nodeR.orderTasks[rLast] = i; /* Indicates that in the rLast-th position, the i-th task is beeing scheduled */

				/* Verifies if it is a leaf node */
				if (remainTaskstoSchedule == 0) {
					/* Verifies if a primal limitant was found */
					if (bestPrimal.sumF2 > nodeR.sumF2) {
						bestPrimal = nodeR;
						timeFoundBestPrimal = GET_TIME(initialTime, clock());
					}
				} else {
					task1 = tasksSortedD1.data();
					task2 = tasksSortedD2.data();

					auxPrimal1 = auxPrimal2 = nodeR;

					/* Founds all tasks not scheduled yet */
					for (k1 = k2 = remainTaskstoSchedule; task1 < endTask1; ++task2, ++task1) {
						/* Verifies if the task1->id-th task is not scheduled yet */
						if ((nodeR.tasks & (1 << task1->id)) == 0) {
							if (minDFound == false) {
								/* from all the tasks not scheduled yet, get the one with the small d1 value */
								minD1 = task1->d1;
								minDFound = true;
							}

							s1 += k1 * task1->d1 + task1->d2;

							auxPrimal1.f1 += task1->d1;
							auxPrimal1.f2 = MAX(auxPrimal1.f1, auxPrimal1.f2) + task1->d2;
							auxPrimal1.sumF2 += auxPrimal1.f2;
							auxPrimal1.orderTasks[numTasks-k1] = char(task1->id);

							--k1;
						}

						/* Verifies if the task2->id-th task is not scheduled yet */
						if ((nodeR.tasks & (1 << task2->id)) == 0) {
							s2 += k2 * task2->d2;

							auxPrimal2.f1 += task2->d1;
							auxPrimal2.f2 = MAX(auxPrimal2.f1, auxPrimal2.f2) + task2->d2;
							auxPrimal2.sumF2 += auxPrimal2.f2;
							auxPrimal2.orderTasks[numTasks-k2] = char(task2->id);

							--k2;
						}
					}

					s1 += remainTaskstoSchedule * nodeR.f1;
					s2 += remainTaskstoSchedule * MAX(nodeR.f2, nodeR.f1 + minD1);

					estimatedSumF2 = nodeR.sumF2 + MAX(s1, s2);

					/* Verifies if a best dual was found */
					if (bestDual.sumF2 > nodeR.sumF2 + s1) {
						bestDual.sumF2 = nodeR.sumF2 + s1;
						timeFoundBestDual = GET_TIME(initialTime, clock());
					}

					if (bestDual.sumF2 > nodeR.sumF2 + s2) {
						bestDual.sumF2 = nodeR.sumF2 + s2;
						timeFoundBestDual = GET_TIME(initialTime, clock());
					}

					/* Verifies if a best primal was found */
					if (bestPrimal.sumF2 > auxPrimal1.sumF2) {
						bestPrimal = auxPrimal1;
						timeFoundBestPrimal = GET_TIME(initialTime, clock());
					}

					if (bestPrimal.sumF2 > auxPrimal2.sumF2) {
						bestPrimal = auxPrimal2;
						timeFoundBestPrimal = GET_TIME(initialTime, clock());
					}

					nodeR.estimate = estimatedSumF2;
					/* Bound step (via primal limitant) */
					if (bestPrimal.sumF2 > estimatedSumF2) activeNodes->push(nodeR);
				}

				/* Verifies if the number of explored nodes was reached */
				if (limitExploredNodes > numExploredNodes) ++numExploredNodes;
				else return;
			}
		}
	}
}
