#include "Notebook.hpp"
#include <exception>
using namespace std;
namespace ariel
{
    static const int HUNRED = 100;
    static bool isValidInputWrite(int page, int row, int col, Direction dir, string const &text)
    {
        /*page, row, col mast be positive*/
        if (page < 0 || row < 0 || col < 0)
        {
            throw invalid_argument("page, row, col mast be positive");
            return false;
        }

        /*string mustn't contain not-printable char*/
        for (size_t i = 0; i < text.length(); i++)
        {
            if (isprint(text.at(i)) == 0)
            {
                throw invalid_argument("text contains not printable char");
            }
        }

        /*string length + col > 100*/
        if (int(text.length()) + col > HUNRED)
        {
            throw out_of_range("text is out of line range");
        }

        /*can't write '~'*/
        if ((text.find('~') != string::npos))
        {
            throw invalid_argument("text contains '~'");
        }
        return true;
    }

    static bool isValidInputRead_Erase(int page, int row, int col, Direction dir, int len)
    {
        /*page, row, col, len mast be positive*/
        if (page < 0 || row < 0 || col < 0 || len < 0)
        {
            throw invalid_argument("page, row, col, len mast be positive");
        }

        /*try to read more than 100 chars in Horizontal (len+col < 100)*/
        if (len + col > HUNRED && dir == Direction::Horizontal)
        {
            throw out_of_range("can't read out of line");
        }
        return true;
    }

    Notebook::Notebook() {}
    Notebook::~Notebook() {}

    void Notebook::write(int page, int row, int col, Direction dir, string const &text)
    {
        if (isValidInputWrite(page, row, col, dir, text))
        {
            char ch = ' ';
            if (dir == Direction::Horizontal)
            {
                for (size_t i = (size_t)col, j = 0; i < text.length() + (size_t)col; i++, j++)
                {
                    ch = this->_notebook[page][row][i];
                    if (isdigit(ch) != 0 || isalpha(ch) != 0 || ch == '~')
                    {
                        throw runtime_error("can't write on written/erased place");
                        exit(0);
                    }
                    this->_notebook[page][row][i] = text[j];
                }
            }
            else
            {
                for (size_t i = (size_t)row, j = 0; i < text.length() + (size_t)row; i++, j++)
                {
                    ch = this->_notebook[page][i][col];
                    if (isprint(ch) != 0 && ch != '_')
                    {
                        throw runtime_error("can't write on written/erased place");
                        exit(0);
                    }
                    this->_notebook[page][i][col] = text[j];
                }
            }
        }
    }

    string Notebook::read(int page, int row, int col, Direction dir, int len) const
    {
        if (isValidInputRead_Erase(page, row, col, dir, len))
        {
            char ch = '_';
            string ans = string("");
            if (dir == Direction::Horizontal)
            {
                for (size_t i = (size_t)col; i < len + col; i++)
                {
                    try
                    {
                        ch = this->_notebook.at(page).at(row)[i];
                    }
                    catch (const std::exception &e)
                    {
                        ch = '_';
                    }
                    if (isdigit(ch) != 0 || isalpha(ch) != 0 || ch == '~')
                    {
                        ans.push_back(ch);
                    }
                    else
                    {
                        ans.push_back('_');
                    }
                }
            }
            else
            {
                for (size_t i = (size_t)row; i < len + row; i++)
                {
                    try
                    {
                        ch = this->_notebook.at(page).at(i)[col];
                    }
                    catch (const std::exception &e)
                    {
                        ch = '_';
                    }
                    if (isdigit(ch) != 0 || isalpha(ch) != 0 || ch == '~')
                    {
                        ans.push_back(ch);
                    }
                    else
                    {
                        ans.push_back('_');
                    }
                }
            }
            return ans;
        }
        throw invalid_argument("invalid_argument of read");
    }

    void Notebook::erase(int page, int row, int col, Direction dir, int len)
    {
        if (isValidInputRead_Erase(page, row, col, dir, len))
        {
            if (dir == Direction::Horizontal)
            {
                for (size_t i = (size_t)col; i < len + col; i++)
                {

                    this->_notebook[page][row][i] = '~';
                }
            }
            else
            {
                for (size_t i = (size_t)row; i < len + row; i++)
                {
                    this->_notebook[page][i][col] = '~';
                }
            }
        }
    }

    void Notebook::show(int page) const{
        for (size_t i = 0; i < HUNRED; i++)
        {
            cout <<i << ": " <<this->read(page, i, 0, Direction::Horizontal, HUNRED) <<endl;
        }
        
    }
}