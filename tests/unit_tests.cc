#include "gtest/gtest.h"

#include "../src/model/regex.hpp"
#include "../src/model/k_strings.hpp"
#include "../src/model/dna_search.hpp"
#include "../src/model/window_substring.hpp"
#include "../src/model/sequence_alignment.hpp"

TEST(RabinKarpTest, BasicSearch) {
    s21::RabinKarp rk;
    rk.SetText("abcdeabcde");
    rk.SetPattern("cde");
    std::list<int> positions = rk.GetPositions();
    ASSERT_EQ(positions.size(), 2);
    ASSERT_EQ(positions.front(), 2);
    ASSERT_EQ(positions.back(), 7);
}

TEST(RabinKarpTest, SearchWithFile) {
    s21::RabinKarp rabinKarp;
    rabinKarp.SetText("../datasets/dna_search_text.txt");
    rabinKarp.SetPattern("../datasets/dna_search_pattern.txt");
    std::list<int> positions = rabinKarp.GetPositions();
    ASSERT_EQ(positions.size(), 2);
    ASSERT_EQ(positions.front(), 65);
    ASSERT_EQ(positions.back(), 9150);
}

TEST(RabinKarpTest, EmptySearch) {
    s21::RabinKarp rk;
    rk.SetText("abcde");
    rk.SetPattern("xyz");
    std::list<int> positions = rk.GetPositions();
    ASSERT_TRUE(positions.empty());
}

TEST(NeedlemanWunschTest, ExampleTest) {
    s21::NeedlemanWunsch nw;
    nw.SetMatchScore(2);
    nw.SetMismatchScore(-1);
    nw.SetGapScore(-2);
    nw.SetSeq("AGTACG", "AGCTCG");
    s21::NeedlemanWunsch::Sequences result = nw.GetOptimalAlignment();
    ASSERT_EQ(result.optimal_score, 6);
    ASSERT_EQ(result.alignment_a, "AG-TACG");
    ASSERT_EQ(result.alignment_b, "AGCT-CG");
}

TEST(NeedlemanWunschTest, FileInputTest) {
    s21::NeedlemanWunsch nw;
    nw.ReadFile("../datasets/sequence_alignment.txt");
    s21::NeedlemanWunsch::Sequences result = nw.GetOptimalAlignment();
    ASSERT_EQ(result.optimal_score, 10);
    ASSERT_EQ(result.alignment_a, "GGGCGACACTCCACCATAGA-");
    ASSERT_EQ(result.alignment_b, "GG-CGACAC-CCACCATACAT");
}

TEST(RegexTest, EmptyStringAndEmptyExpression) {
    s21::Regex reg;
    reg.SetString("");
    reg.SetExpression("");
    ASSERT_TRUE(reg.IsMatch());
}

TEST(RegexTest, NonEmptyStringAndEmptyExpression) {
    s21::Regex reg;
    reg.SetString("abc");
    reg.SetExpression("");
    ASSERT_FALSE(reg.IsMatch());
}

TEST(RegexTest, EmptyStringAndNonEmptyExpression) {
    s21::Regex reg;
    reg.SetString("");
    reg.SetExpression("a*");
    ASSERT_FALSE(reg.IsMatch());
}

TEST(RegexTest, SimpleMatch) {
    s21::Regex reg;
    reg.SetString("ab");
    reg.SetExpression("ab");
    ASSERT_TRUE(reg.IsMatch());
}

TEST(RegexTest, DotWildcard) {
    s21::Regex reg;
    reg.SetString("abc");
    reg.SetExpression("a.c");
    ASSERT_TRUE(reg.IsMatch());
}

TEST(RegexTest, AsteriskQuantifier) {
    s21::Regex reg;
    reg.SetString("aab");
    reg.SetExpression("a*b");
    ASSERT_TRUE(reg.IsMatch());
}

TEST(RegexTest, PlusQuantifier) {
    s21::Regex reg;
    reg.SetString("aab");
    reg.SetExpression("a+b");
    ASSERT_TRUE(reg.IsMatch());
}

TEST(RegexTest, QuestionMarkQuantifier) {
    s21::Regex reg;
    reg.SetString("abc");
    reg.SetExpression("a?bc");
    ASSERT_TRUE(reg.IsMatch());
}

TEST(RegexTest, ComplexExpression) {
    s21::Regex reg;
    reg.SetString("aaabcc");
    reg.SetExpression("a+b.c*");
    ASSERT_TRUE(reg.IsMatch());
}

TEST(RegexTest, FileInputTest) {
    s21::Regex reg;
    reg.ReadFile("../datasets/regex_text.txt");
    ASSERT_TRUE(reg.IsMatch());
}

TEST(KStringTest, AnagramsWithNoDifference) {
    s21::KString ks;
    ks.SetStrings("listen", "listen");
    ASSERT_EQ(ks.GetDiffCount(), 0);
}

TEST(KStringTest, AnagramsWithOneDifference) {
    s21::KString ks;
    ks.SetStrings("listen", "nistel");
    ASSERT_EQ(ks.GetDiffCount(), 1);
}

TEST(KStringTest, NonAnagrams) {
    s21::KString ks;
    ks.SetStrings("hello", "world");
    ASSERT_EQ(ks.GetDiffCount(), -1);
}

TEST(KStringTest, FileInputTest) {
    s21::KString ks;
    ks.ReadFile("../datasets/k_strings.txt");
    ASSERT_EQ(ks.GetDiffCount(), 3);
}

TEST(WindowSubstringTest, MinimumWindowSubstringExist) {
    s21::WindowSubstring ws;
    ws.SetString("ADOBECODEBANC");
    ws.SetPattern("ABC");
    ASSERT_EQ(ws.GetMinimumWindowSubstring(), "BANC");
}

TEST(WindowSubstringTest, MinimumWindowSubstringNotExist) {
    s21::WindowSubstring ws;
    ws.SetString("ADOBECODEBANC");
    ws.SetPattern("XYZ");
    ASSERT_EQ(ws.GetMinimumWindowSubstring(), "");
}

TEST(WindowSubstringTest, MinimumWindowSubstringSameStringAndPattern) {
    s21::WindowSubstring ws;
    ws.SetString("hello");
    ws.SetPattern("hello");
    ASSERT_EQ(ws.GetMinimumWindowSubstring(), "hello");
}

TEST(WindowSubstringTest, EmptyStringOrPattern) {
    s21::WindowSubstring ws;
    ws.SetString("");
    ws.SetPattern("ABC");
    ASSERT_EQ(ws.GetMinimumWindowSubstring(), "");
    ws.SetString("ADOBECODEBANC");
    ws.SetPattern("");
    ASSERT_EQ(ws.GetMinimumWindowSubstring(), "");
}

TEST(WindowSubstringTest, FileInputTest) {
    s21::WindowSubstring ws;
    ws.ReadFile("../datasets/window_substrings.txt");
    ASSERT_EQ(ws.GetMinimumWindowSubstring(), "GACACCCACCATACAT");
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
