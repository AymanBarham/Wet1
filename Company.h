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
    shared_ptr<Employee> highestSalaryEmp;
};


#endif //WET1_COMPANY_H
