cmake_minimum_required(VERSION 3.12)
project(CPP_Test_Task_Menti VERSION 1.1.0)

add_executable (Test_Task_Menti_CPP main.cpp)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Test_Task_Menti_CPP PROPERTY CXX_STANDARD 20)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION}) 
