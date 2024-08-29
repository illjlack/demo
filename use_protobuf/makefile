# 指定C++编译器为g++
CXX = g++

# 编译器标志：
# -std=c++11: 使用C++11标准
# -Wall: 打开所有警告
# -I: 指定protobuf头文件的搜索路径
CXXFLAGS = -std=c++11 -Wall -I$(CURDIR)/third_party/protobuf/include

# 链接器标志：
# -L: 指定protobuf库文件的搜索路径
# -lprotobuf: 链接protobuf库
# -Wl,-rpath: 将库路径嵌入到可执行文件中
LDFLAGS = -L$(CURDIR)/third_party/protobuf/lib -lprotobuf -Wl,-rpath,$(CURDIR)/third_party/protobuf/lib

# 目标可执行文件的名称
TARGET = protobuf_demo

# 源文件列表，包括生成的person.pb.cc
SRCS = main.cpp person.pb.cc

# 将源文件转换为对象文件的规则
OBJS = $(SRCS:.cpp=.o)

# 链接阶段：将所有对象文件链接为可执行文件
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) -o $(TARGET)

# 编译阶段：将每个源文件编译为对象文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	rm -f $(TARGET) $(OBJS)
