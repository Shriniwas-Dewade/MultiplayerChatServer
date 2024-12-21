add_executable(server main.cpp server.cpp) #file_transfer.cpp

target_link_libraries(server PUBLIC -lpthread)
