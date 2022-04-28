cmake_minimum_required(VERSION 3.21)
project(Wet1)

set(CMAKE_CXX_STANDARD 11)

add_executable(Wet1 Employee.h library1.h Company.h CompareEmpByID.h CompareEmpBySalary.h CompareCompanyByID.h DataStructure.h AVLTree.h library1.cpp main1.cpp)
