// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "scheduler.h"

using std::cout;
using std::endl;

int main()
{
    // List of available employees
    vector<Employee> employees = {
        Employee(1, certifiedInstaller, available),
        Employee(2, certifiedInstaller, available),
        Employee(3, certifiedInstaller, available),
        Employee(4, certifiedInstaller, available),
        Employee(5, certifiedInstaller, available),
        Employee(12, certifiedInstaller, available),
        Employee(14, certifiedInstaller, available),
        Employee(15, certifiedInstaller, available),
        Employee(6, installerPendingCertification, available),
        Employee(7, installerPendingCertification, available),
        Employee(8, installerPendingCertification, available),
        Employee(9, installerPendingCertification, available),
        Employee(10, installerPendingCertification, available),
        Employee(17, installerPendingCertification, available),
        Employee(18, installerPendingCertification, available),
        Employee(19, installerPendingCertification, available),
        Employee(20, installerPendingCertification, available),
        Employee(11, laborer, available),
        Employee(12, laborer, available),
        Employee(13, laborer, available),
        Employee(14, laborer, available),
        Employee(15, laborer, available),
        Employee(21, laborer, available),
        Employee(22, laborer, available),
        Employee(23, laborer, available),
        Employee(24, laborer, available),
        Employee(25, laborer, available),
        Employee(26, laborer, available)
    };

    // List of buildings that need PV installation
    vector<Building> buildingsList = {
        Building(1, singleStoryRes, high),
        Building(2, doubleStoryRes, high),
        Building(3, commercial, high),
        Building(4, doubleStoryRes, high),
        Building(5, singleStoryRes, high),
        Building(6, doubleStoryRes, high),
        Building(7, commercial, medium),
        Building(8, doubleStoryRes, medium),
        Building(9, singleStoryRes, medium),
        Building(10, commercial, medium),
        Building(11, doubleStoryRes, medium),
        Building(12, singleStoryRes, medium),
        Building(13, doubleStoryRes, medium),
        Building(14, commercial, low),
        Building(15, doubleStoryRes, low),
        Building(16, singleStoryRes, low),
        Building(17, commercial, low),
        Building(18, doubleStoryRes, low),
        Building(19, commercial, low),
        Building(20, singleStoryRes, low),
        Building(21, commercial, low),
        Building(22, doubleStoryRes, low),
        Building(23, commercial, low),
        Building(24, doubleStoryRes, low),
        Building(25, doubleStoryRes, low)
    };

    /* Key - Value pairs to store results 
    * key<int> - Building ID
    * value<vector<int>> - Array of employee IDs assigned to work on the building
    */
    map<int, vector<int>> result;

    // Queues to classify different types of employees
    queue<Employee> certified;
    queue<Employee> certPending;
    queue<Employee> laborers;

    // Queue to store buildings according to their priority
    queue<Building> buildings;

    // Classify employees as per their authorization
    splitEmployeeList(employees, certified, certPending, laborers);

    // Insert buildings to the queue according to priority
    listToQueue(buildingsList, buildings);

    /*Step1: For each day of week
    * Step 2: Check if there are buildings in the queue
    * Step 3: Check if enough employees are available
    * Step 4: Assign employees to the buildings
    * Step 5: Modify queues to account for assigned buildings and workers
    */
    for (days day = Mon; day < weekend; day = days(day + 1)) // Start scheduling for each working day
    {
        while (!buildings.empty()) // Keep assigning until all buildings in queue are assigned
        {
            Building building = buildings.front(); // First building in the "priority" queue
            if (building.getStatus() == unassigned) 
            {
                // Check if there are enough installers available for particular type of building
                if (building.getType() == commercial && checkRequired(building, certified, certPending, laborers)) 
                {
                    // Assign employees with proper authorization to particular buildings
                    assign(result, building, certified, 2);
                    assign(result, building, certPending, 2);

                    // Opttional assignment priority: Laborers > Installers Pending Certification > Certified Installers
                    if (laborers.size() >= 4) 
                    {
                        assign(result, building, laborers, 4);
                    }
                    else if (certPending.size() >= 6) 
                    {
                        assign(result, building, certPending, 4);
                    }
                    else if (certified.size() >= 6) 
                    {
                        assign(result, building, certified, 4);
                    }
                    else
                        /* if not enough installers are available, override the priority
                        * and "re-enque" the building for scheduling further in the same week
                        */
                    {
                        cout << "Building " << building.getId() << " can not be scheduled for work on " << day <<".";
                        swapPriority(buildings);
                        break;
                    }

                    buildings.pop(); // Once scheduled for installation, deque the building from queue
                }

                else if (building.getType() == doubleStoryRes && checkRequired(building, certified, certPending, laborers)) 
                {
                    assign(result, building, certified, 1);

                    if (laborers.size() >= 1) 
                    {
                        assign(result, building, laborers, 1);
                    }
                    else 
                    {
                        assign(result, building, certPending, 1);
                    }

                    buildings.pop();
                }

                else if (building.getType() == singleStoryRes && checkRequired(building, certified, certPending, laborers)) 
                {
                    assign(result, building, certified, 1);
                    buildings.pop();
                }

                else 
                {
                    cout << "Building " << building.getId() << " can not be scheduled for work on " << day << ".\n";
                    swapPriority(buildings);
                    break;
                }
            }

            else 
            {
                cout << "Building " << building.getId() << " has already been scheduled." << endl;               
            }

        }

        // Show the schedule for week
        cout << "Schedule for " << day << " : " << endl;

        cout << "(Building Number : Worker IDs)" << endl;

        for (auto const& kv : result) 
        {
            cout << kv.first << " : ";
            for (auto const& i : kv.second) 
            {
                cout << " " << i;
            }
            cout << endl;
        }
        cout << endl;

        // Clear results and queue for reusing on the next day
        result.clear();

        clearQueue(certified);
        clearQueue(certPending);
        clearQueue(laborers);
        
        splitEmployeeList(employees, certified, certPending, laborers);
    }

}
