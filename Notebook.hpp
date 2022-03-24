#ifndef NOTEBOOK_HPP
#define NOTEBOOK_HPP
#include <iostream>
#include <unordered_map>
#include <vector>
#include <initializer_list>
#include "Direction.hpp"
using namespace std;

namespace ariel
{
    class Page
    {
    private:
        int _last;
        unordered_map<int, char*> _page;
    public:
        Page();
        Page(int lines);
        ~Page();
        unordered_map<int, char*> getPage() const;
        void setPage(int lines);
        void show(int start, int end) const;
    };

    class Notebook
    {
    private:
        int _last;
        typedef tuple<int, int> location;
        unordered_map<location, char*> _notebook;

    public:
        Notebook();
        ~Notebook();
        void write(int, int, int, Direction, string const &);
        string read(int page, int row, int col, Direction dir, int len) const;
        void erase(int page, int row, int col, Direction dir, int len);
        void show(int page) const;
    };


    
}
#endif