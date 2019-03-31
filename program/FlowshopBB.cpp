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

FlowshopBB::FlowshopBB(int _limitExploredNodes, int _maximumAllowedTime) {
	initialTime = clock(); /* initial execution time */

	numExploredNodes = 0;
	limitExploredNodes = _limitExploredNodes;
	maximumAllowedTime = (float) _maximumAllowedTime;

	bestNode = Node(0, 0, INT_MAX);

	dominance = new std::map<int, std::pair<int, int>>();
	activeNodes = new std::map<int, std::queue<Node>>();
}

void FlowshopBB::addTask(int d1, int d2) {
	tasks.emplace_back((char) tasks.size(), d1, d2);
	tasksSortedD1.emplace_back((char) tasksSortedD1.size(), d1, d2);
	tasksSortedD2.emplace_back((char) tasksSortedD2.size(), d1, d2);
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

	/* Explore the tree */
	while(activeNodes->size() > 0 && maximumAllowedTime > getTime(initialTime, clock())) {
		auto topNode = activeNodes->begin();

		Node currentNode = topNode->second.front();
		topNode->second.pop();

		if (topNode->second.size() == 0) activeNodes->erase(topNode);

		int taskNum = (int) currentNode.tasks.to_ulong();

		/* applies dominance relation */
		if (dominance->find(taskNum) != dominance->end()) {
			auto &ref = (*dominance)[taskNum];
			if (ref.first <= currentNode.f2 && ref.second <= currentNode.sumF2) continue;
			else {
				ref.first = currentNode.f2;
				ref.second = currentNode.sumF2;
			}
		} else dominance->insert(std::make_pair(taskNum, std::make_pair(currentNode.f2, currentNode.sumF2)));

		/* Branch step (Expand all currentNode's children) */
		for (char i = 0; i < numTasks; ++i) {
			/* Verifies if task i should be scheduled */
			if (currentNode.tasks[i] == 0) {
				Task &taskR = tasks[i];
				Node nodeR = currentNode;

				nodeR.order.push_back(char(i));
				nodeR.tasks[i] = 1; /* Marks task i as scheduled */
				nodeR.f1 = currentNode.f1 + taskR.d1;
				nodeR.f2 = std::max(nodeR.f1, currentNode.f2) + taskR.d2;
				nodeR.sumF2 = currentNode.sumF2 + nodeR.f2;

				int f2 = 0;
				int minD1 = 0;
				int s1 = 0, s2 = 0;
				bool minDFound = false;
				char r = nodeR.tasks.count();

				if (r == numTasks) {
					/* verifies if primal limitant was found */
					if (bestNode.sumF2 > nodeR.sumF2) bestNode = nodeR;
				} else {
					for (char j = 0, k1 = r + 1, k2 = r + 1; j < numTasks; ++j) {
						Task &task1 = tasksSortedD1[j];
						Task &task2 = tasksSortedD2[j];

						if (nodeR.tasks[task1.id] == 0) {
							if (minDFound == false) {
								minD1 = task1.d1;
								minDFound = true;
							}

							s1 += (numTasks - k1 + 1) * task1.d1 + task1.d2;
							++k1;
						}

						if (nodeR.tasks[task2.id] == 0) {
							s2 += (numTasks - k2 + 1) * task2.d2;
							++k2;
						}
					}

					s1 += (numTasks - r) * nodeR.f1;
					s2 += (numTasks - r) * std::max(nodeR.f2, nodeR.f1 + minD1);

					int estimatedF = nodeR.sumF2 + std::max(s1, s2);

					/* Bound step (via primal limitant) */
					if (bestNode.sumF2 > estimatedF)
						(*activeNodes)[estimatedF].push(nodeR);
				}

				++numExploredNodes;

				if (numExploredNodes > limitExploredNodes || maximumAllowedTime < getTime(initialTime, clock())) return;
			}
		}
	}
}

int FlowshopBB::getExploredNodes() {
	return numExploredNodes;
}

Node FlowshopBB::getBestNode() {
	return bestNode;
}
