#pragma once
#include <iostream>
#include <unordered_map>
#include "Direction.hpp"
#include<exception>
using namespace std;
namespace ariel
{
    class Notebook
    {
    private:
        unordered_map<int, unordered_map<int, char[100]>> _notebook;

    public:
        Notebook();
        ~Notebook();
        void write(int, int, int, Direction, string const &);
        string read(int page, int row, int col, Direction dir, int len) const;
        void erase(int page, int row, int col, Direction dir, int len);
        void show(int page) const;
    };

}