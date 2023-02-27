#pragma once

#include <iostream>
#include <memory>
#include <vector>

using namespace std;

template<typename T>
class MemHolder
{
public:
    MemHolder()
        :maxSize(100000)
    {
        if (maxSize > 0)
        {
            cout << "construct object, allocate memory." << endl;
            dataArray = new T[maxSize];
            memset(dataArray, T(0), maxSize * sizeof(T));
        }
    }
    ~MemHolder()
    {
        if (dataArray)
        {
            cout << "destruct object, release memory." << endl;
            delete[] dataArray;
        }
    }
    void notify() const
    {
        cout << "This is Memholder Object." << endl;
    }

private:
    T* dataArray = nullptr;
    const size_t maxSize;
};

class TestData
{
public:
    TestData()
    {
        data = make_unique<MemHolder<double>>();
    }
    ~TestData()
    {

    }

    const MemHolder<double>* getHolder()
    {
        return data.get();
    }

private:
    unique_ptr<MemHolder<double>> data;
};