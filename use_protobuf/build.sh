set -e  # 如果脚本中的任何命令出错，立即退出
set -x  # 显示执行的每一条命令（用于调试）

# 定义 protobuf 的根目录
PROTOBUF_ROOT=$(pwd)/third_party/protobuf
# 定义根目录（根据实际路径设置）
ROOT=$(pwd)/

# 创建一个用于存放第三方库的目录
mkdir -p third_party

# 进入 third_party 目录
cd third_party

# 检查 protobuf 目录中是否已存在 protoc 文件，如果不存在，则执行安装步骤
if [ ! -f protobuf/bin/protoc ]; then
    # 检查 protobuf 源码包是否存在，不存在则下载
    if [ ! -f protobuf-cpp-3.20.0.tar.gz ]; then
        wget https://github.com/protocolbuffers/protobuf/releases/download/v3.20.0/protobuf-cpp-3.20.0.tar.gz
    fi

    # 解压 protobuf 源码包
    tar zxvf protobuf-cpp-3.20.0.tar.gz
    cd protobuf-3.20.0

    # 配置并安装 protobuf，安装路径设置为 ../protobuf
    ./autogen.sh  # 生成配置脚本
    ./configure --prefix=`pwd`/../protobuf
    make -j1
    make install  # 安装编译后的文件

    # 返回 third_party 目录
    cd ../
fi

# 返回上一层目录
cd ..

# 使用自定义的 protoc 路径生成 C++ 源代码
$PROTOBUF_ROOT/bin/protoc -I$PROTOBUF_ROOT/include -I$ROOT -I. --cpp_out=. $ROOT/person.proto

# 执行项目的 make 命令，进行编译
make

# 返回最终的退出状态码
exit $?
