#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/Array.hpp>
#include <plx/literal/InputStream.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    class InputStream_Test : public PlxTestFixture {};

    Array* positionArray(int line, int col, int pos);

    TEST_F(InputStream_Test, CreateInstance) {
        InputStream* inputStream1 = new InputStream("");
        EXPECT_TRUE(inputStream1->isA(TypeId::D_INPUTSTREAM));
        EXPECT_EQ("InputStream", inputStream1->typeName());
    }

    TEST_F(InputStream_Test, BoolValue) {
        InputStream* inputStream1 = new InputStream("");
        EXPECT_FALSE(inputStream1->boolValue());
        InputStream* inputStream2 = new InputStream("a");
        EXPECT_TRUE(inputStream2->boolValue());
        inputStream2->next();
        EXPECT_FALSE(inputStream2->boolValue());
    }

    TEST_F(InputStream_Test, IsEmpty) {
        InputStream* inputStream1 = new InputStream("");
        EXPECT_TRUE(inputStream1->isEmpty());
        InputStream* inputStream2 = new InputStream("a");
        EXPECT_FALSE(inputStream2->isEmpty());
        inputStream2->next();
        EXPECT_TRUE(inputStream2->isEmpty());
    }

    TEST_F(InputStream_Test, Next) {
        InputStream* inputStream1 = new InputStream("abc");
        EXPECT_EQ('a', inputStream1->next());
        EXPECT_EQ('b', inputStream1->next());
        EXPECT_EQ('c', inputStream1->next());
        EXPECT_EQ(0, inputStream1->next());
    }
    
    TEST_F(InputStream_Test, Next_Empty) {
        InputStream* inputStream1 = new InputStream("");
        EXPECT_TRUE(inputStream1->isEmpty());
        char c1 = inputStream1->next();
        EXPECT_TRUE(inputStream1->isEmpty());
        EXPECT_EQ(0, c1);
        char c2 = inputStream1->next();
        EXPECT_TRUE(inputStream1->isEmpty());
        EXPECT_EQ(0, c2);
    }

    TEST_F(InputStream_Test, Position) {
        InputStream* inputStream1 = new InputStream("ab\ncd\nef");
        Array* position = inputStream1->position();
        Array* expectedPosition = positionArray(1, 0, 0);
        EXPECT_TRUE(position->equals(expectedPosition));

        (void)inputStream1->next();  // 'a'
        position = inputStream1->position();
        expectedPosition = positionArray(1, 1, 1);
        EXPECT_TRUE(position->equals(expectedPosition));

        (void)inputStream1->next();  // 'b'
        position = inputStream1->position();
        expectedPosition = positionArray(1, 2, 2);
        EXPECT_TRUE(position->equals(expectedPosition));

        (void)inputStream1->next();  // '\n'
        position = inputStream1->position();
        expectedPosition = positionArray(1, 3, 3);
        EXPECT_TRUE(position->equals(expectedPosition));

        (void)inputStream1->next();  // 'c'
        position = inputStream1->position();
        expectedPosition = positionArray(2, 1, 4);
        EXPECT_TRUE(position->equals(expectedPosition));

        (void)inputStream1->next();  // 'd'
        position = inputStream1->position();
        expectedPosition = positionArray(2, 2, 5);
        EXPECT_TRUE(position->equals(expectedPosition));
        
        (void)inputStream1->next();  // '\n'
        position = inputStream1->position();
        expectedPosition = positionArray(2, 3, 6);
        EXPECT_TRUE(position->equals(expectedPosition));

        (void)inputStream1->next();  // 'e'
        position = inputStream1->position();
        expectedPosition = positionArray(3, 1, 7);
        EXPECT_TRUE(position->equals(expectedPosition));

        (void)inputStream1->next();  // 'e'
        position = inputStream1->position();
        expectedPosition = positionArray(3, 2, 8);
        EXPECT_TRUE(position->equals(expectedPosition));

    }

    TEST_F(InputStream_Test, ShowOn) {
        InputStream* inputStream1 = new InputStream("");
        std::stringstream ss;
        ss << inputStream1;
        EXPECT_NE(0, ss.str().length());
    }

}
