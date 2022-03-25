#include "doctest.h"
#include "Notebook.hpp"
using namespace ariel;


TEST_CASE("BAD_INPUT"){
    Notebook note1;
    SUBCASE("WRITE"){
        /*page, row, col mast be positive*/
        for (size_t i = 0; i < 30; i++)
        {
            int v1 = (rand() % 100 + 1)*-1;
            CHECK_THROWS_MESSAGE(note1.write(v1, 0, 0, Direction::Horizontal, "hi"), "PAGE must be positive");
            CHECK_THROWS_MESSAGE(note1.write(0, v1, 0, Direction::Horizontal, "hi"), "ROW must be positive");
            CHECK_THROWS_MESSAGE(note1.write(0, 0, v1, Direction::Horizontal, "hi"), "COL must be positive");
        }

        /*string must contain printable char [33,126] */
        CHECK_THROWS_MESSAGE(note1.write(0, 0, 0, Direction::Horizontal, ""), "invalid string");
        CHECK_THROWS_MESSAGE(note1.write(0, 0, 0, Direction::Horizontal, "\n"), "invalid string");
        CHECK_THROWS_MESSAGE(note1.write(0, 0, 0, Direction::Horizontal, "\t"), "invalid string");
        CHECK_THROWS_MESSAGE(note1.write(0, 0, 0, Direction::Horizontal, "\r"), "invalid string");

        /*string length + col > 100*/
        string str = string("");
        for (size_t i = 0; i <= 100; i++)
        {
            str.append("a");
        }
        CHECK_THROWS_MESSAGE(note1.write(0, 0, 0, Direction::Horizontal, str), "string length is out of bounds - 100");
        CHECK_THROWS_MESSAGE(note1.write(0, 0, 99, Direction::Horizontal, "aaa"), "string length is out of bounds, try change COL or STR");
        CHECK_THROWS_MESSAGE(note1.write(0, 0, 100, Direction::Horizontal, "aa"), "string length is out of bounds, try change COL or STR");

        /*Attempt to write in a place that has already been written*/
        note1.write(1, 0, 0, Direction::Horizontal, "a");
        CHECK_THROWS_MESSAGE(note1.write(1, 0, 0, Direction::Horizontal, "a"), "this position is already token");
        note1.erase(1, 0, 0,Direction::Horizontal, 1);
        CHECK_THROWS_MESSAGE(note1.write(1, 0, 0, Direction::Horizontal, "a"), "this position is already token"); //can't even after erase

        /*can't write '~'*/
        CHECK_THROWS_MESSAGE(note1.write(0, 0, 8, Direction::Horizontal, "~"), "can't write '~'");
        CHECK_THROWS_MESSAGE(note1.write(0, 8, 8, Direction::Horizontal, "ab~cd"), "can't write '~'");


    }

    SUBCASE("READ"){
        /*page, row, col, len mast be positive*/
        for (size_t i = 0; i < 30; i++)
        {
            int v1 = (rand() % 100 + 1)*-1;
            CHECK_THROWS_MESSAGE(note1.read(v1, 0, 0, Direction::Horizontal, 1), "PAGE must be positive");
            CHECK_THROWS_MESSAGE(note1.read(0, v1, 0, Direction::Horizontal, 1), "ROW must be positive");
            CHECK_THROWS_MESSAGE(note1.read(0, 0, v1, Direction::Horizontal, 1), "COL must be positive");
            CHECK_THROWS_MESSAGE(note1.read(0, 0, 0, Direction::Horizontal, v1), "LEN must be positive"); 
        }
        CHECK_THROWS_MESSAGE(note1.read(0, 0, 0, Direction::Horizontal, 0), "LEN must be positive");

        /*try to read more than 100 chars (len+col < 100)*/
        CHECK_THROWS_MESSAGE(note1.read(0, 0, 0, Direction::Horizontal, 101), "LEN can't be >100");
        CHECK_THROWS_MESSAGE(note1.read(0, 0, 100, Direction::Horizontal, 2), "LEN is out of bounds - try change COL or LEN");
        CHECK_THROWS_MESSAGE(note1.read(0, 0, 50, Direction::Horizontal, 51), "LEN is out of bounds - try change COL or LEN");


    }

    SUBCASE("ERASE"){
        /*page, row, col, len mast be positive*/
        for (size_t i = 0; i < 30; i++)
        {
            int v1 = (rand() % 100 + 1)*-1;
            CHECK_THROWS_MESSAGE(note1.erase(0, v1, 0, Direction::Horizontal, 1), "ROW must be positive");
            CHECK_THROWS_MESSAGE(note1.erase(0, 0, v1, Direction::Horizontal, 1), "COL must be positive");
            CHECK_THROWS_MESSAGE(note1.erase(v1, 0, 0, Direction::Horizontal, 1), "PAGE must be positive");
            CHECK_THROWS_MESSAGE(note1.erase(0, 0, 0, Direction::Horizontal, v1), "LEN must be positive"); 
        }
        CHECK_THROWS_MESSAGE(note1.erase(0, 0, 0, Direction::Horizontal, 0), "LEN must be positive");

        /*try to erase more than 100 chars (len+col < 100)*/
        CHECK_THROWS_MESSAGE(note1.erase(0, 0, 0, Direction::Horizontal, 101), "LEN can't be >100");
        CHECK_THROWS_MESSAGE(note1.erase(0, 0, 100, Direction::Horizontal, 2), "LEN is out of bounds - try change COL or LEN");
        CHECK_THROWS_MESSAGE(note1.erase(0, 0, 50, Direction::Horizontal, 51), "LEN is out of bounds - try change COL or LEN");

    }

}

TEST_CASE("GOOD_INPUT"){
    Notebook note1;
    note1.write(0, 0, 0, Direction::Vertical, "abcde");
    CHECK(note1.read(0, 0, 0, Direction::Vertical, 5) == "abcde");
    note1.erase(0, 2, 0, Direction::Vertical, 2);
    CHECK(note1.read(0, 0, 0, Direction::Vertical, 5) == "ab~~e"); //after erase
    CHECK(note1.read(0, 0, 0, Direction::Vertical, 10) == "ab~~e_____"); //both written & not-written chars
    CHECK(note1.read(0, 0, 8, Direction::Vertical, 10) == "__________"); //created line, not-written chars
    note1.write(100,100,50, Direction::Horizontal, "abcd");
    CHECK(note1.read(100, 99, 51, Direction::Vertical, 3) == "_b_"); //write Horizontal, read Vertical
    note1.erase(100, 99, 51, Direction::Vertical, 3);
    CHECK(note1.read(100, 99, 50, Direction::Horizontal, 3) == "_~_");
    CHECK(note1.read(100, 100, 50, Direction::Horizontal, 3) == "a~c");
    CHECK(note1.read(100, 101, 50, Direction::Horizontal, 3) == "_~_");
    CHECK(note1.read(1500, 6,3, Direction::Horizontal, 10) == "__________");
    CHECK(note1.read(1499, 4,3, Direction::Vertical, 10) == "__________");

}