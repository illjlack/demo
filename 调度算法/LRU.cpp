/*
#include <iostream>
#include <unordered_map>
#include <list>
#include <stdexcept> // 引入 std::out_of_range

template <typename K, typename V>
class LRUCache {
public:
    LRUCache(int capacity) : capacity(capacity) {}

    // 获取缓存中的值
    V get(const K& key) {
        // 检查键是否存在
        if (cacheMap.find(key) == cacheMap.end()) {
            throw std::out_of_range("Key not found"); // 键不存在，抛出异常
        }
        // 将访问的项移动到链表的前面（表示最近使用）
        moveToFront(key);
        return cacheMap[key].first; // 返回对应的值
    }

    // 将键值对插入缓存
    void put(const K& key, const V& value) {
        // 如果键已经存在，更新值并将项移动到链表的前面
        if (cacheMap.find(key) != cacheMap.end()) {
            cacheMap[key].first = value;
            moveToFront(key);
        } else {
            // 如果缓存已满，移除最久未使用的项
            if (cacheList.size() >= capacity) {
                K oldKey = cacheList.back(); // 获取最久未使用的键
                cacheList.pop_back(); // 移除链表末尾的项
                cacheMap.erase(oldKey); // 从哈希表中移除
            }
            // 将新项添加到链表的前面
            cacheList.push_front(key);
            cacheMap[key] = {value, cacheList.begin()}; // 存储键值对及其在链表中的位置
        }
    }

private:
    int capacity;  // 缓存的最大容量
    std::list<K> cacheList; // 用于保持键的访问顺序的链表
    std::unordered_map<K, std::pair<V, typename std::list<K>::iterator>> cacheMap; // 存储键值对及其在链表中的位置的哈希表

    // 将指定的键移动到链表的前面
    void moveToFront(const K& key) {
        // 从链表中删除当前的键位置
        cacheList.erase(cacheMap[key].second);
        // 将键添加到链表的前面
        cacheList.push_front(key);
        // 更新键在链表中的新位置
        cacheMap[key].second = cacheList.begin();
    }
};

int main() {
    LRUCache<int, std::string> lru(3); // 创建一个容量为 3 的 LRU 缓存，键为 int，值为 std::string

    lru.put(1, "Apple");
    lru.put(2, "Banana");
    lru.put(3, "Cherry");

    std::cout << "获取 1: " << lru.get(1) << std::endl; // 输出 "Apple"
    
    try {
        lru.put(4, "Date"); // 淘汰键 2
        std::cout << "获取 2: " << lru.get(2) << std::endl; // 抛出异常，因为键 2 已被淘汰
    } catch (const std::out_of_range& e) {
        std::cout << "获取 2: " << e.what() << std::endl; // 输出 "Key not found"
    }

    try {
        lru.put(5, "Elderberry"); // 淘汰键 3
        std::cout << "获取 3: " << lru.get(3) << std::endl; // 抛出异常，因为键 3 已被淘汰
    } catch (const std::out_of_range& e) {
        std::cout << "获取 3: " << e.what() << std::endl; // 输出 "Key not found"
    }
    
    std::cout << "获取 4: " << lru.get(4) << std::endl; // 输出 "Date"
    std::cout << "获取 5: " << lru.get(5) << std::endl; // 输出 "Elderberry"

    return 0;
}

*/

// LRU（Least Recently Used）
#include <iostream>
#include <list>
#include <unordered_map>


class LRUCache
{
    public:
        LRUCache(int capacity): m_capacity(capacity) {}
        ~LRUCache(){}

        int get(int key)
        {
            if(m_cacheMap.count(key))
            {
                moveToFront(key);
                return m_cacheMap[key].first;
            }
            else
            {
                return -1;
            }
        }

        void put(int key, int value)
        {
            if(m_cacheMap.count(key))
            {
                m_cacheMap[key].first = value;
                moveToFront(key);
            }
            else
            {
                // 如果缓存已满，移除最久未使用的项
                if (m_lruList.size() >= m_capacity) 
                {
                    int oldKey = m_lruList.back();
                    m_lruList.pop_back();
                    m_cacheMap.erase(oldKey);
                }
                m_lruList.push_front(key);
                m_cacheMap[key] = {value, m_lruList.begin()};
            }
        }

    private:
        int m_capacity;
        std::list<int> m_lruList;
        std::unordered_map<int, std::pair<int, std::list<int>::iterator>> m_cacheMap; // 键映射到值和键在列表中的位置

        void moveToFront(int key) // 把键值对应的列表位置移动到队首  
        {
            m_lruList.erase(m_cacheMap[key].second);
            m_lruList.push_front(key);

            //更新映射保存的位置
            m_cacheMap[key].second=m_lruList.begin();
        }
};

int main()
{
     LRUCache lru(3); // 创建一个容量为 3 的 LRU 缓存

    lru.put(1, 10);
    lru.put(2, 20);
    lru.put(3, 30);

    std::cout << "获取 1: " << lru.get(1) << std::endl; // 输出 10
    lru.put(4, 40); // 淘汰键 2
    std::cout << "获取 2: " << lru.get(2) << std::endl; // 输出 -1 (未找到)

    lru.put(5, 50); // 淘汰键 3
    std::cout << "获取 3: " << lru.get(3) << std::endl; // 输出 -1 (未找到)
    std::cout << "获取 4: " << lru.get(4) << std::endl; // 输出 40
    std::cout << "获取 5: " << lru.get(5) << std::endl; // 输出 50

    return 0;
}