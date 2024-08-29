# 指定编译器
CXX = g++

# 编译选项
CXXFLAGS = -std=c++11 -O2

# 源文件和目标文件
SRC = time_test.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = performance_test

# 默认目标
all: $(TARGET)

# 生成可执行文件
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# 编译源文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	rm -f $(OBJ) $(TARGET)

# 伪目标，防止与文件名冲突
.PHONY: all clean run
