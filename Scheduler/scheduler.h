#pragma once
#include <map>
#include <vector>
#include <queue>

using std::map;
using std::vector;
using std::queue;

/* Enums for storing symbolic constants */

enum days {Mon, Tues, Wed, Thur, Fri, weekend};

enum authorization {certifiedInstaller, installerPendingCertification, laborer};

enum availability {available, busy, onLeave};

enum buildingType {singleStoryRes, doubleStoryRes, commercial};

enum installationStatus {assigned, unassigned};

enum priorityLevel {high = 3, medium = 2, low = 1};

/* Employee class- represents particular installer instance */
class Employee
{
private:
	int id;
	//std::string name;
	authorization _authorization;
	availability _availability;

public:
	Employee(int num, authorization auth, availability av);
	~Employee();
	int getId() const { return id; };
	availability getAvailability() const { return _availability; };
	void setAvailability(availability x) { _availability = x; };
	authorization getAuthorization() const { return _authorization; };
};


/* Building class- represents particular building instance */
class Building
{
private:
	int id;
	buildingType type;
	installationStatus status;
	priorityLevel priority;

public:
	Building(int num, buildingType bType, priorityLevel pLevel);
	~Building();
	int getId() const { return id; };
	buildingType getType() const { return type; };
	installationStatus getStatus() const { return status; };
	void setStatus(installationStatus x) { status = x; };
	priorityLevel getPriority() const { return priority; };
};

// Free function definitions
// More info is in scheduler.cpp file

void assign(map<int, vector<int>>& m, Building& b, queue<Employee>& q, int n);

void swapPriority(queue<Building>& b);

bool checkRequired(Building const& b,
	queue<Employee> const& q1,
	queue<Employee> const& q2,
	queue<Employee> const& q3);

void splitEmployeeList(vector<Employee> const& v,
	queue<Employee>& certInst,
	queue<Employee>& instPendCert,
	queue<Employee>& laborer);

void clearQueue(queue<Employee>& q);

void clearQueue(queue<Building>& q);

void listToQueue(vector<Building>& v, queue<Building>& q);