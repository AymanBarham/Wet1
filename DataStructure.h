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
    AVLTree<Employee, CompareEmpByID> allEmpByID;
    AVLTree<Employee, CompareEmpBySalary> allEmpBySalary;
    AVLTree<Company, CompareCompanyByID> allCompanies;
    AVLTree<Company, CompareCompanyByID> workingCompanies;
    shared_ptr<Employee> highestSalaryEmp;
public:
    DataStructure() =default;
    ~DataStructure() =default;

    StatusType AddCompany(int CompanyID, int Value) {
        if (CompanyID <= 0 || Value <= 0) {
            return INVALID_INPUT;
        }

        try {
            allCompanies.insert(shared_ptr<Company>(new Company(CompanyID, Value)));
        } catch (AVLTree<Company, CompareCompanyByID>::AlreadyExists& e) { // add avl exception here
            return FAILURE;
        } catch (std::runtime_error& error) {
            return ALLOCATION_ERROR;
        } catch (...) {
            return FAILURE;
        }

        return SUCCESS;
    }

    StatusType AddEmployee(int EmployeeID, int CompanyID, int Salary, int Grade) {
        if (EmployeeID <= 0 || CompanyID <= 0 || Salary <= 0 || Grade < 0) {
            return INVALID_INPUT;
        }

        try {
            shared_ptr<Company> tempCompany = shared_ptr<Company>(new Company(CompanyID, 0));
            shared_ptr<Company> empCompany = allCompanies.find(tempCompany);
            shared_ptr<Employee> emp = shared_ptr<Employee>(new Employee(EmployeeID, Salary, Grade, nullptr));
            if (empCompany == nullptr) {
                return FAILURE;
            }

            emp->company = empCompany;

            allEmpByID.insert(emp);
            allEmpBySalary.insert(emp);

            empCompany->employeesByID.insert(emp);
            empCompany->employeesBySalary.insert(emp);
        } catch (AVLTree<Employee, CompareEmpByID>::AlreadyExists& e) {
            return FAILURE;
        } catch (AVLTree<Employee, CompareEmpBySalary>::AlreadyExists& e) {
            return FAILURE;
        } catch (...) {
            return ALLOCATION_ERROR;
        }

        return SUCCESS;
    }

};


#endif //WET1_DATASTRUCTURE_H
