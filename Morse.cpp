#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_map>
using namespace std;

const int CACHE_SIZE = 16; 
const int MEMORY_SIZE = 32;

class Cache
{
    protected:
    vector<int> cache;
    vector<int> memory;
    public:
    Cache(int size)//constructor
    {
        cache.resize(size, -1);
        for (int i = 0; i < MEMORY_SIZE; i++)
        {
            memory.push_back(i);
        }
    }

    virtual ~Cache(){} //virtual destructor

    virtual void accessMemory(int address) = 0; //pure virtual function

    void displayCache()//display the cache
    {
        cout << "Current Cache: ";
        for (int i = 0; i < CACHE_SIZE; i++)
        {
            if (cache[i] == -1)
            {
                cout << "empty ";
            } 
            else
            {
                cout << cache[i] << " ";
            }
        }
        cout << endl;
    }
};

class LRUCache : public Cache
{
    private:
    vector<int> cacheOrder;
    public:
    LRUCache(int size) : Cache(size) {}//calling base class constructor

    void accessMemory(int address) override//overrides base class
    {
        auto it = find(cache.begin(), cache.end(), address);//search address in cache

        if (it != cache.end())//if addres found then it will not reach cache end
        {
            cout << "LRU Cache hit!" << endl;
            cacheOrder.erase(remove(cacheOrder.begin(), cacheOrder.end(), address), cacheOrder.end());
            cacheOrder.push_back(address);//keeping the order of cache updated
        }
        else
        {
            if (cacheOrder.size() == CACHE_SIZE)//address not found
            {
                int removedAddress = cacheOrder.front();//cache is full,apply LRU
                cacheOrder.erase(cacheOrder.begin());
                auto index = find(cache.begin(), cache.end(), removedAddress);
                *index = address;
                cacheOrder.push_back(address);
            }
            else
            {
                cacheOrder.push_back(address);//cache not full,add simply
                auto emptySlot=find(cache.begin(), cache.end(), -1);//insert add at first empty slot
                *emptySlot = address;
                if (address > 0)//principle of locality
                {
                    int prevAddress = address - 1;
                    auto prevSlot = find(cache.begin(), cache.end(), prevAddress);
                    if (prevSlot == cache.end())
                    {
                        cacheOrder.push_back(prevAddress);//address not in cache,add it
                        auto emptySlot = find(cache.begin(), cache.end(), -1);
                        *emptySlot = prevAddress;
                    }
                }
                if (address < MEMORY_SIZE - 1)
                {
                    int nextAddress = address + 1;
                    auto nextSlot = find(cache.begin(), cache.end(), nextAddress);
                    if (nextSlot == cache.end())
                    {
                        cacheOrder.push_back(nextAddress);//address not in cache,add it
                        auto emptySlot = find(cache.begin(), cache.end(), -1);
                        *emptySlot = nextAddress;
                    }
                }
            }
            cout << "LRU Cache miss - Address " << address << " added to the cache." << endl;
        }
    }
};

class LFUCache : public Cache
{
    private:
    unordered_map<int, int> frequency;
    public:
    LFUCache(int size) : Cache(size) {}//calling base class constructor

    void accessMemory(int address) override
    {
        auto it = find(cache.begin(), cache.end(), address);
        if (it != cache.end())
        {
            cout << "LFU Cache hit!" << endl;//address found in the cache
            frequency[address]++;
        }
        else
        {
            if (cache.size() == CACHE_SIZE)//address not in the cache
            {
                int minFreqAddress = cache.front(); //cache is full,apply LFU replacement
                for (int val : cache)
                {
                    if (frequency[val] < frequency[minFreqAddress])
                    {
                        minFreqAddress = val;
                    }
                }
                auto index = find(cache.begin(), cache.end(), minFreqAddress);
                *index = address;
                frequency[address] = 1;
            }
            else
            {
                cache.push_back(address);//cache is not full,simply add the address
                frequency[address] = 1;
                if (address > 0)//add nearby addresses according to locality
                {
                    int prevAddress = address - 1;
                    auto prevSlot = find(cache.begin(), cache.end(), prevAddress);
                    if (prevSlot == cache.end())
                    {
                        cache.push_back(prevAddress);//address not in cache, add it
                        frequency[prevAddress] = 1;
                    }
                }
                if (address < MEMORY_SIZE - 1)
                {
                    int nextAddress = address + 1;
                    auto nextSlot = find(cache.begin(), cache.end(), nextAddress);
                    if (nextSlot == cache.end())
                    {
                        cache.push_back(nextAddress);//address not in cache, add it
                        frequency[nextAddress] = 1;
                    }
                }
            }
            cout << "LFU Cache miss - Address " << address << " added to the cache." << endl;
        }
    }
};

int main()
{
    Cache* cachePolicy = nullptr;//nitialize a pointer to Cache, initially set to nullptr
    cout << "Cache size: " << CACHE_SIZE << endl;
    while (true)
    {
        cout << "Options:\n1. Access Memory Address\n2. Display Cache\n3. Change Cache Replacement Policy\n4. Exit\n";
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        if (choice == 1)
        {
            int address;
            cout << "Enter a memory address to access (0-" << MEMORY_SIZE - 1 << "): ";
            cin >> address;
            if (address < 0 || address >= MEMORY_SIZE)
            {
                cout << "Invalid memory address." << endl;
                continue;
            }
            if (cachePolicy)
                cachePolicy->accessMemory(address);//access memory based on the selected policy
            else
                cout << "Cache is not initialized. Please select a replacement policy." << endl;
        }
        else if (choice == 2)
        {
            if (cachePolicy)
                cachePolicy->displayCache();//display the current cache
            else
                cout << "Cache is not initialized. Please select a replacement policy." << endl;
        }
        else if (choice == 3)
        {
            if (cachePolicy)
            {
                delete cachePolicy;//clean up the previous cache to avoid memory leaks
            }
            int replacementChoice;
            cout << "Select Cache Replacement Policy:\n1. LRU\n2. LFU\nEnter your choice: ";
            cin >> replacementChoice;
            if (replacementChoice == 1)
            {
                cachePolicy = new LRUCache(CACHE_SIZE);//create an LRU cache
                cout << "LRU Cache selected." << endl;
            }
            else if (replacementChoice == 2)
            {
                cachePolicy = new LFUCache(CACHE_SIZE);//create an LFU cache
                cout << "LFU Cache selected." << endl;
            }
            else
            {
                cout << "Invalid replacement policy choice. Using default LRU Cache." << endl;
                cachePolicy = new LRUCache(CACHE_SIZE);//use LRU as the default cache policy
            }
        }
        else if (choice == 4)
        {
            if (cachePolicy)
            {
                delete cachePolicy;//clean up dynamic memory to prevent memory leaks
            }
            break;//exit the program
        }
        else
        {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
    return 0;
}
