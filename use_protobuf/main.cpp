#include <iostream>
#include <fstream>
#include "person.pb.h"

void serializePerson(const std::string& filename)
{
    // 创建一个 Person 对象并赋值
    Person person;
    person.set_name("John Doe");
    person.set_id(123);
    person.set_email("johndoe@example.com");

    // 将对象序列化为二进制数据并保存到文件
    std::ofstream output(filename, std::ios::binary);
    if (!person.SerializeToOstream(&output))
    {
        std::cerr << "Failed to write person to file." << std::endl;
    }
}

void deserializePerson(const std::string& filename)
{
    // 从文件中读取二进制数据并反序列化为 Person 对象
    Person person;
    std::ifstream input(filename, std::ios::binary);
    if (!person.ParseFromIstream(&input))
    {
        std::cerr << "Failed to read person from file." << std::endl;
    } 
    else 
    {
        // 输出反序列化得到的数据
        std::cout << "Name: " << person.name() << std::endl;
        std::cout << "ID: " << person.id() << std::endl;
        std::cout << "Email: " << person.email() << std::endl;
    }
}

int main() {
    const std::string filename = "person.bin";

    serializePerson(filename);    // 序列化并保存到文件
    deserializePerson(filename);  // 从文件反序列化并输出内容

    return 0;
}
