//
// Created by Ayman Barham on 20/04/2022.
//

#ifndef WET1_COMPANY_H
#define WET1_COMPANY_H

#include <iostream>
#include "Employee.h"
#include "CompareEmpByID.h"
#include "CompareEmpBySalary.h"

using std::shared_ptr;

class Company {
public:
    int id;
    int value;
    AVLTree<shared_ptr<Employee>, CompareEmpByID> employeesByID;
    AVLTree<shared_ptr<Employee>, CompareEmpBySalary> employeesBySalary;
    shared_ptr<Employee> highestSalaryEmpInCompany;

    Company(int id, int value): id(id), value(value) {}

    friend bool operator==(const Company& c1, const Company& c2);

    bool isWorking() const {
        return !employeesByID.isEmpty();
    }

    void addEmployee(shared_ptr<Employee> toAdd) {
        employeesByID.add(toAdd);
        employees
    }
};

bool operator==(const Company& c1, const Company& c2) {
    return c1.id == c2.id;
}


#endif //WET1_COMPANY_H
