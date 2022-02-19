cmake_minimum_required(VERSION 3.13)

project("Finite state tool")

include_directories(Automation_base)
link_directories(Automation_base)

add_library(AutomationBase STATIC Automation_base/AutomationBase.h Automation_base/AutomationBase.cpp)
add_library(CustomSet STATIC Collections/Custom_set/CustomSet.hpp)
add_library(Commands STATIC CommandShell/Commands/Commands.h CommandShell/Commands/Commands.cpp)
add_library(CommandsDispatcher STATIC CommandShell/Dispatcher/CommandDispatcher.h CommandShell/Dispatcher/CommandDispatcher.cpp)
add_library(Environment STATIC  Environment/AFL_Environment.h  Environment/AFL_Environment.cpp)
add_library(Fsa STATIC  Finate_state_automation/FiniteStateAutomation.h  Finate_state_automation/FiniteStateAutomation.cpp)
add_library(GraphVis STATIC GraphVisualizationTool/GraphVisualiser.h GraphVisualizationTool/GraphVisualiser.cpp)
add_library(Npda STATIC Non-deterministic_pushdown_automaton/NPDA.h  Non-deterministic_pushdown_automaton/NPDA.cpp)

set_target_properties(CustomSet PROPERTIES LINKER_LANGUAGE CXX)

add_executable(FST FstDispatcher/main.cpp)

target_link_libraries(FST CustomSet CommandsDispatcher Commands Environment Fsa GraphVis Npda AutomationBase)

file(COPY GraphVisualizationTool/header.x
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY GraphVisualizationTool/footer.x
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})