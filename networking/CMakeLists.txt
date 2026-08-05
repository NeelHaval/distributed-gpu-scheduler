add_library(networking
    src/Socket.cpp
    src/Client.cpp
    src/Server.cpp
)

target_include_directories(networking PUBLIC include)

target_link_libraries(networking
    PUBLIC
    ws2_32
)