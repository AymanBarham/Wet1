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
//#include "CompareEmpBySalary.h"
#include "AVLTree.h"
#include "library1.h"

class DataStructure {
    AVLTree<Employee, CompareEmpByID> allEmpByID;
    AVLTree<Employee, CompareEmpBySalary> allEmpBySalary;
    AVLTree<Company, CompareCompanyByID> allCompanies;
    AVLTree<Company, CompareCompanyByID> workingCompanies;
    //shared_ptr<Employee> highestSalaryEmp;
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
            shared_ptr<Employee> emp = shared_ptr<Employee>(new Employee(EmployeeID, Grade, Salary, nullptr));
            if (empCompany == nullptr) {
                return FAILURE;
            }

            emp->company = empCompany;

            allEmpByID.insert(emp);
            allEmpBySalary.insert(emp);

            empCompany->employeesByID.insert(emp);
            empCompany->employeesBySalary.insert(emp);

            if (!workingCompanies.find(empCompany)) {
                workingCompanies.insert(empCompany);
            }
        } catch (AVLTree<Employee, CompareEmpByID>::AlreadyExists& e) {
            return FAILURE;
        } catch (AVLTree<Employee, CompareEmpBySalary>::AlreadyExists& e) {
            return FAILURE;
        } catch (...) {
            return ALLOCATION_ERROR;
        }

        return SUCCESS;
    }

    StatusType RemoveCompany(int CompanyID) {
        if (CompanyID <= 0) {
            return INVALID_INPUT;
        }
        try {
            shared_ptr<Company> toRemoveCompany = shared_ptr<Company>(new Company(CompanyID, 0));
            if (workingCompanies.find(toRemoveCompany)) {
                return FAILURE;
            }

            allCompanies.remove(toRemoveCompany);

        } catch (...) { // only throw possible is memory
            return ALLOCATION_ERROR;
        }

        return SUCCESS;
    }

    StatusType RemoveEmployee(int EmployeeID) {
        if (EmployeeID <= 0) {
            return INVALID_INPUT;
        }

        try {
            shared_ptr<Employee> emp = shared_ptr<Employee>(new Employee(EmployeeID, 0, 0, nullptr));

            shared_ptr<Employee> foundEmp = allEmpByID.find(emp);
            if (!foundEmp) {
                return FAILURE;
            }

            allEmpByID.remove(foundEmp);
            allEmpBySalary.remove(foundEmp);
            foundEmp->company->employeesByID.remove(foundEmp);
            foundEmp->company->employeesBySalary.remove(foundEmp);

            if (foundEmp->company->employeesByID.isEmpty()) {
                workingCompanies.remove(foundEmp->company);
            }
        } catch (...) { // only possible exception is memory.
            return ALLOCATION_ERROR;
        }

        return SUCCESS;
    }

    StatusType GetCompanyInfo(int CompanyID, int *Value, int *NumEmployees) {
        if (CompanyID <= 0 || !Value || !NumEmployees) {
            return INVALID_INPUT;
        }
        try {
            shared_ptr<Company> tempCompany = shared_ptr<Company>(new Company(CompanyID, 0));
            shared_ptr<Company> foundCompany = allCompanies.find(tempCompany);
            if (!foundCompany) {
                return FAILURE;
            }
            *NumEmployees = foundCompany->employeesByID->getSize();
            *Value = foundCompany->value;
        } catch (...) { // only possible exception is memory.
            return ALLOCATION_ERROR;
        }
        return SUCCESS;
    }


    StatusType GetEmployeeInfo(int EmployeeID, int *EmployerID, int *Salary, int *Grade){
        if(EmployeeID <= 0 || !EmployerID || !Salary || !Grade){
            return INVALID_INPUT;
        }
        try {
            shared_ptr<Employee> emp = shared_ptr<Employee>(new Employee(EmployeeID, 0, 0, nullptr));

            shared_ptr<Employee> foundEmp = allEmpByID.find(emp);
            if (!foundEmp) {
                return FAILURE;
            }
            *EmployerID = foundEmp->company->id;
            *Salary = foundEmp->salary;
            *Grade = foundEmp->grade;
        }
        catch (...){// only possible exception is memory.
            return ALLOCATION_ERROR;
        }
        return SUCCESS;
    }

    StatusType IncreaseCompanyValue(int CompanyID, int ValueIncrease){
        if(CompanyID <= 0 || ValueIncrease <= 0){
            return INVALID_INPUT;
        }
        try {
            shared_ptr<Company> tempCompany = shared_ptr<Company>(new Company(CompanyID, 0));
            shared_ptr<Company> foundCompany = allCompanies.find(tempCompany);
            if (!foundCompany) {
                return FAILURE;
            }
            foundCompany->value += ValueIncrease;
        } catch (...) { // only possible exception is memory.
            return ALLOCATION_ERROR;
        }
        return SUCCESS;
    }

    StatusType PromoteEmployee(int EmployeeID, int SalaryIncrease, int BumpGrade){
        if(EmployeeID <= 0 || SalaryIncrease <= 0){
            return INVALID_INPUT;
        }
        try {
            shared_ptr<Employee> emp = shared_ptr<Employee>(new Employee(EmployeeID, 0, 0, nullptr));

            shared_ptr<Employee> foundEmp = allEmpByID.find(emp);
            if (!foundEmp) {
                return FAILURE;
            }
            foundEmp->salary += SalaryIncrease;
            if(BumpGrade > 0)
            {
                foundEmp->grade++;
            }
        }
        catch (...){// only possible exception is memory.
            return ALLOCATION_ERROR;
        }
        return SUCCESS;
    }

    StatusType HireEmployee(int EmployeeID, int NewCompanyID){
        if(EmployeeID <= 0 || NewCompanyID <= 0){
            return INVALID_INPUT;
        }
        try{
            shared_ptr<Employee> emp = shared_ptr<Employee>(new Employee(EmployeeID, 0, 0, nullptr));
            shared_ptr<Employee> foundEmp = allEmpByID.find(emp);
            if (!foundEmp) {
                return FAILURE;
            }
            shared_ptr<Company> tempCompany = shared_ptr<Company>(new Company(NewCompanyID, 0));
            shared_ptr<Company> newCompany = allCompanies.find(tempCompany);
            if (!newCompany) {
                return FAILURE;
            }
            if(foundEmp->company == newCompany){
                return FAILURE;
            }
            StatusType flag = RemoveEmployee(foundEmp->id);
            if(flag != SUCCESS){
                return flag;
            }
            flag = AddEmployee(EmployeeID , NewCompanyID , foundEmp->salary , foundEmp->grade);
            if(flag != SUCCESS){
                return flag;
            }
        }catch (...){
            return ALLOCATION_ERROR;
        }
        return SUCCESS;
    }
    StatusType GetHighestEarner(int CompanyID, int *EmployeeID){
        if(CompanyID == 0 || !EmployeeID){
            return INVALID_INPUT;
        }
        if(CompanyID < 0){
            if(allCompanies.isEmpty()){
                return FAILURE;
            }
            *EmployeeID = allEmpBySalary.getMax()->id;
        }else {
            try {
                shared_ptr<Company> tempCompany = shared_ptr<Company>(new Company(CompanyID, 0));
                shared_ptr<Company> newCompany = allCompanies.find(tempCompany);
                if (!newCompany) {
                    return FAILURE;
                }
                if(newCompany->employeesByID->isEmpty()){
                    return FAILURE;
                }
                *EmployeeID = newCompany->employeesBySalary->getMax()->id;
            }catch (...){
                return ALLOCATION_ERROR;
            }
        }
        return SUCCESS;
    }
};


#endif //WET1_DATASTRUCTURE_H
