#include "Notebook.hpp"
#include <stdexcept>

namespace ariel
{
    static const int COLS = 100;
    bool validInputWrite(int page, int row, int col, Direction dir, string const &text)
    {
        /*page, row, col mast be positive*/
        if (page < 0 || row < 0 || col < 0)
        {
            throw invalid_argument("page, row, col mast be positive");
        }

        /*string must contain printable char [33,126] */
        // for (int i = 0; i < 32; i++)
        // { 
        //     char ch = i;
        //     if ((text.find(ch) != string::npos))
        //     {
        //         throw invalid_argument("text contains not printable char");
        //     }
            
        // }

        /*string length + col > 100*/
        if(int(text.length()) + col > COLS){
            throw out_of_range("text is out of line range");
        }

        /*can't write '~'*/
        if ((text.find('~') != string::npos))
            {
                throw invalid_argument("text contains '~'");
            }
        return true;
    }

    bool validInputRead_Erase(int page, int row, int col, Direction dir, int len)
    {
        /*page, row, col, len mast be positive*/
        if (page < 0 || row < 0 || col < 0 || len <0)
        {
            throw invalid_argument("page, row, col, len mast be positive");
        }

        /*try to read more than 100 chars in Horizontal (len+col < 100)*/
        if (len + col >COLS && dir == Direction::Horizontal)
        {
            throw out_of_range("can't read out of line");
        }
        return true;

    }


    void Page::setPage(int lines)
    {
        cout<<"set page\n";
        if (lines < 0)
        {
            throw invalid_argument("lines must be positive");
        }

        int start = this->_last == 0 ? 0 : this->_last + 1;
        for (int i = start; i <= lines; i++)
        {
            char* line;
            fill(line, line + COLS, '_');
            this->_page.insert(make_pair(i, line));
        }
        this->_last = lines;
    }

    unordered_map<int, char *> Page::getPage() const
    {
        return this->_page;
    }

    void Page::show(int start, int end) const
    {
        if (this->_last <1)
        {
            cout <<"page is empty\n";
            return;
        }
        if (this->_last < end)
        {
            throw out_of_range("this row doesn't exist");
            return;
        }
        
        
        
        for (int i = start; i <= end; i++)
        {
            int j=0;
            cout << i << ": ";
            for(j=0; j<COLS; j++){
                char* a = this->_page.find(i)->second;
                cout << a[j];
            }
            cout <<"\n";
        }
    }

    Page::Page(int lines)
    {
        cout <<"costruct page\n";
        this->_last = 0;
        this->setPage(lines);
    }

    Page::Page(){
        this->_last=0;
    }

    Page::~Page()
    {
        cout << "page destroyed\n";
    }

    
}