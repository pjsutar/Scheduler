#include "scheduler.h"

/* Employee class methods */
// Constructor
Employee::Employee(int num,
	authorization auth, 
	availability av) : id(num), _authorization(auth), _availability(av) {
	
}

// Destructor
Employee::~Employee() {

}

/* Building class methods */
// Constructor
Building::Building(int num,
	buildingType bType,
	priorityLevel pLevel) : id(num), type(bType), priority(pLevel) {
	status = unassigned;
}

// Destructor
Building::~Building() {

}

// Free functions

// Function to assign installers to work on buildings and update the schedule
void assign(map<int, vector<int>>& m, Building& b, queue<Employee>& q, int n) {
	int bId = b.getId();
	for (int i = 0; i < n; i++) {
		Employee toAssign = q.front();
		m[bId].push_back(toAssign.getId());
		//toAssign.setAvailability(busy); // This can be invoked to account for employee unavailability
		q.pop();
	}
	b.setStatus(assigned);
}

// Function to adjust priority
// Usage - remove buildings that can not be assigned today and re-insert in queue
void swapPriority(queue<Building>& b) {
	Building& toSwap = b.front();
	b.push(toSwap);
	b.pop();
}

// Function to check if there are enough authorized employees available to work on the type of building
bool checkRequired(Building const& b,
	queue<Employee> const& q1,
	queue<Employee> const& q2,
	queue<Employee> const& q3) {
	if (b.getType() == commercial) {
		return (q1.size() >= 2 && q2.size() >= 2) && (q3.size() >= 4 || q1.size() >= 6 || q2.size() >= 6);
	}
	else if (b.getType() == doubleStoryRes) {
		return (q1.size() >= 1) && (q2.size() >= 1 || q3.size() >= 1);
	}
	else {
		return (q1.size() >= 1);
	}
}

// Classify employees according to their authorization
void splitEmployeeList(vector<Employee> const& v,
	queue<Employee>& certInst,
	queue<Employee>& instPendCert,
	queue<Employee>& laborer) {

	for (auto& employee : v) {
		if (employee.getAuthorization() == certifiedInstaller) {
			certInst.push(employee);
		}
		else if (employee.getAuthorization() == installerPendingCertification) {
			instPendCert.push(employee);
		}
		else {
			laborer.push(employee);
		}
	}
}

// function to generate queue from vector
void listToQueue(vector<Building>& v, queue<Building>& q) {
	for (auto& building : v) {
		q.push(building);
	}
}

// Function to clear queues
void clearQueue(queue<Employee>& q) {
	while (!q.empty()) {
		q.pop();
	}
}

// Function overload of clear queue function for Building queue
void clearQueue(queue<Building>& q) {
	while (!q.empty()) {
		q.pop();
	}
}