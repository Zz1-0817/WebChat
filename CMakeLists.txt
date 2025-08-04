cmake_minimum_required(VERSION 3.10)

project(WebChat)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    ./
    ./Socket
    ./SQLite
    ./Utility
    ./Http)

add_library(sqlite3 STATIC
    ./sqlite3/sqlite3.c
)

# 设置 SQLite 包含目录
target_include_directories(sqlite3 PUBLIC
    ${CMAKE_SOURCE_DIR}/sqlite3
)

# 添加编译选项（可选）
target_compile_options(sqlite3 PRIVATE
    -DSQLITE_ENABLE_FTS5
    -DSQLITE_ENABLE_JSON1
)

# 查找 Threads 模块
find_package(Threads REQUIRED)

set(HTTP_SOURCE ./Http/http_msg.cpp ./Http/http_handle.cpp)
set(SQLITE_SOURCE ./SQLite/SQLhandle.cpp)
set(SOCKET_SOURCE ./Socket/Socket.h)
set(U_SOURCE ./Utility/Jsonhandle.cpp ./Utility/UserInfo.cpp ./Utility/TimeH.cpp ./Utility/MsgInfo.cpp ./Utility/ChatRoomInfo.cpp)

add_executable(${CMAKE_PROJECT_NAME} 
${SOCKET_SOURCE}
${SQLITE_SOURCE}
${U_SOURCE}
${HTTP_SOURCE}
./Server/Chatserver.cpp)


# 在目标上链接线程库
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Threads::Threads)


# 链接 SQLite 库
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE sqlite3)


target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE dl)