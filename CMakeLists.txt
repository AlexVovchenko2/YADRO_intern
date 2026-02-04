# CMakeList.txt: проект CMake для YADRO_intern; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project ("YADRO_intern")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (YADRO_intern "main.cpp" "EventAndTable.h" "Exceptions.h" "functions.cpp")

target_link_options(YADRO_intern PRIVATE -static-libgcc -static-libstdc++)

if(WIN32)
    set_target_properties(YADRO_intern PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET YADRO_intern PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
