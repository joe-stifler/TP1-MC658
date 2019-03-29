#include <bits/stdc++.h>

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
public:
    FlowshopBB() {
        numExploredNodes = 0;
    }

    void addTask(int d1, int d2) {
		tasks.emplace_back(tasks.size(), d1, d2);
    }

    void run() {
		std::sort(tasks.begin(), tasks.end());

    }

private:
	int numExploredNodes;
    std::vector<Task> tasks;
	// std::map<int, std::bitset<int>> exploredNodes;
};



int main() {
    FlowshopBB fbb;

	fbb.addTask(2, 1);
	fbb.addTask(3, 1);
	fbb.addTask(2, 3);

    fbb.run();

    return 0;
}
