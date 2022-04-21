//
// Created by Ayman Barham on 20/04/2022.
//

#ifndef WET1_DATASTRUCTURE_H
#define WET1_DATASTRUCTURE_H

#include <iostream>
using std::shared_ptr;

#include "Company.h"
#include "Employee.h"
#include "CompareCompanyByID.h"
#include "CompareEmpByID.h"
#include "CompareEmpBySalary.h"
#include "AVLTree.h"
#include "library1.h"

class DataStructure {
    AVLTree<shared_ptr<Employee>, CompareEmpByID> allEmpByID;
    AVLTree<shared_ptr<Employee>, CompareEmpBySalary> allEmpBySalary;
    AVLTree<shared_ptr<Company>, CompareCompanyByID> allCompanies;
    AVLTree<shared_ptr<Company>, CompareCompanyByID> workingCompanies;
    shared_ptr<Employee> highestSalaryEmp;
public:
    DataStructure() =default;
    ~DataStructure() =default;

    StatusType AddCompany(int CompanyID, int Value) {
        if (CompanyID <= 0 || Value <= 0) {
            return INVALID_INPUT;
        }

        try {
            allCompanies.add(shared_ptr<Company>(new Company(CompanyID, Value)));
        } catch (std::exception& e) { // add avl exception here
            return FAILURE;
        } catch (std::runtime_error& error) {
            return ALLOCATION_ERROR;
        }

        return SUCCESS;
    }

    StatusType AddEmployee(int EmployeeID, int CompanyID, int Salary, int Grade) {
        if (EmployeeID <= 0 || CompanyID <= 0 || Salary <= 0 || Grade < 0) {
            return INVALID_INPUT;
        }

        shared_ptr<Company> company = allCompanies.findByID(CompanyID);
        shared_ptr<Employee> emp = shared_ptr<Employee>(Employee(EmployeeID,0,0, nullptr));
        if (allEmpByID.find(emp) == nullptr || company == nullptr) {
            return FAILURE;
        }
    }

};


#endif //WET1_DATASTRUCTURE_H
