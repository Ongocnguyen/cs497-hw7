#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/literal/String.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    class String_Test : public PlxTestFixture {};

    TEST_F(String_Test, CreateInstance) {
        String* abc = new String("abc");
        EXPECT_TRUE(abc->isA(TypeId::L_STRING));
        EXPECT_EQ("String", abc->typeName());
        EXPECT_EQ("abc", abc->value());
    }

    TEST_F(String_Test, BoolValue) {
        String* empty = new String("");
        String* notEmpty = new String("abc");
        EXPECT_FALSE(empty->boolValue());
        EXPECT_TRUE(notEmpty->boolValue());
    }

    TEST_F(String_Test, Equals) {
        String* s1a = new String("abc");
        String* s1b = new String("abc");
        String* s2 = new String("xyz");
        EXPECT_TRUE(s1a->equals(s1b));
        EXPECT_TRUE(s1b->equals(s1a));
        EXPECT_FALSE(s2->equals(s1a));
        EXPECT_FALSE(s1b->equals(s2));
    }

    TEST_F(String_Test, HashCode) {
        const int N_STRINGS = 6;
        String* strings[N_STRINGS] = {
            new String(""),
            new String(" "),
            new String("a"),
            new String("A"),
            new String("ab"),
            new String("abc")
        };
        HashCode hashCodes[N_STRINGS];
        for (int n=0; n<N_STRINGS; n++) {
            EXPECT_TRUE(strings[n]->hashCode(hashCodes[n]));
        }
        for (int n=0; n<N_STRINGS-1; n++) {
            for (int m=n+1; m<N_STRINGS; m++) {
                EXPECT_NE(hashCodes[n], hashCodes[m]);
            }
        }
        String* strings2[N_STRINGS] = {
            new String(""),
            new String(" "),
            new String("a"),
            new String("A"),
            new String("ab"),
            new String("abc")
        };
        for (int n=0; n<N_STRINGS; n++) {
            HashCode hashCode;
            EXPECT_TRUE(strings2[n]->hashCode(hashCode));
            EXPECT_EQ(hashCodes[n], hashCode);
        }
    }

    TEST_F(String_Test, DisplayOn) {
        {
            std::stringstream ss;
            String* s = new String("");
            s->displayOn(ss);
            EXPECT_EQ("", ss.str());
        }
        {
            std::stringstream ss;
            String* s = new String("abc");
            s->displayOn(ss);
            EXPECT_EQ("abc", ss.str());
        }
    }

    TEST_F(String_Test, ShowOn) {
        {
            std::stringstream ss;
            String* s = new String("");
            ss << s;
            EXPECT_EQ("\"\"", ss.str());
        }
        {
            std::stringstream ss;
            String* s = new String("abc");
            ss << s;
            EXPECT_EQ("\"abc\"", ss.str());
        }
    }

}
