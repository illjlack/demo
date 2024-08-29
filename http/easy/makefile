# 指定编译器
CXX = g++

# 编译选项
CXXFLAGS = -Wall -std=c++11

# 目标可执行文件的名称
TARGET = http_server

# 源文件列表
SRCS = main.cpp http_server.cpp

# 生成的目标文件列表
OBJS = $(SRCS:.cpp=.o)

# 目标规则
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# 依赖关系规则
main.o: main.cpp http_server.h
	$(CXX) $(CXXFLAGS) -c main.cpp

http_server.o: http_server.cpp http_server.h
	$(CXX) $(CXXFLAGS) -c http_server.cpp

# 清理生成文件
clean:
	rm -f $(TARGET) $(OBJS)
