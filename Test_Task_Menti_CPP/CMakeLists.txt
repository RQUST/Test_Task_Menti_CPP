# CMakeList.txt: проект CMake для Test_Task_Menti_CPP; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.
add_executable (Test_Task_Menti_CPP "Test_Task_Menti_CPP.cpp" "Test_Task_Menti_CPP.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Test_Task_Menti_CPP PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
