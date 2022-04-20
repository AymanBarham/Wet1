
#ifndef WET1_EMPLOYEE_H
#define WET1_EMPLOYEE_H

#include <iostream>
#include "Company.h"
using std::shared_ptr;

class Employee {
public:
    int id;
    int grade;
    int salary;
    shared_ptr<Company> company;
};


#endif //WET1_EMPLOYEE_H
