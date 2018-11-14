/*### Bank OCR

Your manager has recently purchased a machine that assists in reading letters and faxes sent in by branch offices. The machine scans the paper documents, and produces a file with a number of entries. You will write a program to parse this file.

#### Specification
#### User Story 1

The following format is created by the machine:
```
    _  _     _  _  _  _  _
  | _| _||_||_ |_   ||_||_|
  ||_  _|  | _||_|  ||_| _|
```
Each entry is 3 lines long, and each line has 27 characters. 3 lines of each entry contain an account number written using pipes and underscores.

Each account number should have 9 digits, all of which should be in the range 0-9. A normal file contains around 500 entries.

Write a program that can take this file and parse it into actual account numbers.

Example input and output
```
 _  _  _  _  _  _  _  _  _
| || || || || || || || || |
|_||_||_||_||_||_||_||_||_|

=> 000000000

  |  |  |  |  |  |  |  |  |
  |  |  |  |  |  |  |  |  |

=> 111111111

 _  _  _  _  _  _  _  _  _
 _| _| _| _| _| _| _| _| _|
|_ |_ |_ |_ |_ |_ |_ |_ |_

=> 222222222

 _  _  _  _  _  _  _  _  _
 _| _| _| _| _| _| _| _| _|
 _| _| _| _| _| _| _| _| _|

=> 333333333

|_||_||_||_||_||_||_||_||_|
  |  |  |  |  |  |  |  |  |

=> 444444444

 _  _  _  _  _  _  _  _  _
|_ |_ |_ |_ |_ |_ |_ |_ |_
 _| _| _| _| _| _| _| _| _|

=> 555555555

 _  _  _  _  _  _  _  _  _
|_ |_ |_ |_ |_ |_ |_ |_ |_
|_||_||_||_||_||_||_||_||_|

=> 666666666

 _  _  _  _  _  _  _  _  _
  |  |  |  |  |  |  |  |  |
  |  |  |  |  |  |  |  |  |

=> 777777777

 _  _  _  _  _  _  _  _  _
|_||_||_||_||_||_||_||_||_|
|_||_||_||_||_||_||_||_||_|

=> 888888888

 _  _  _  _  _  _  _  _  _
|_||_||_||_||_||_||_||_||_|
 _| _| _| _| _| _| _| _| _|

=> 999999999

    _  _     _  _  _  _  _
  | _| _||_||_ |_   ||_||_|
  ||_  _|  | _||_|  ||_| _|

=> 123456789
```
*/
#include <gtest/gtest.h>
#include <string>

std::vector<std::string> SplitStringForLines(const std::string& string)
{
    std::vector<std::string> lines;

    std::string mutableString = string;
    while (true)
    {
        size_t newLinePos = mutableString.find("\n");
        lines.push_back(mutableString.substr(0, newLinePos));

        if (newLinePos == std::string::npos)
        {
            break;
        }

        mutableString = mutableString.substr(newLinePos + 1);
    }

    return lines;
}

std::vector<std::string> ExtractDigitsFromEntity(const std::string& entity)
{
    std::vector<std::string> lines = SplitStringForLines(entity);
    lines.resize(3);

    std::vector<std::string> digits;

    for (size_t i = 0; i < lines[0].size(); i += 3)
    {
        std::string digit;
        for (auto& line: lines)
        {
            digit += line.substr(i, 3);
        }

        digits.push_back(digit);
    }

    return digits;
}

TEST(SplitStringForLines, OneLineWithoutDelimiters)
{
    std::vector<std::string> lines = {"a"};
    EXPECT_EQ(lines, SplitStringForLines("a"));
}

TEST(SplitStringForLines, TwoLinesWithOneDelimiters)
{
    std::vector<std::string> lines = {"a", "b"};
    EXPECT_EQ(lines, SplitStringForLines("a\nb"));
}

TEST(SplitStringForLines, ThreeLinesWithTwoDelimiters)
{
    std::vector<std::string> lines = {"a", "b", "c"};
    EXPECT_EQ(lines, SplitStringForLines("a\nb\nc"));
}

TEST(SplitStringForLines, OneLineForEmptyLine)
{
    std::vector<std::string> lines = {""};
    EXPECT_EQ(lines, SplitStringForLines(""));
}

TEST(SplitStringForLines, TwoEmptyLinesForLineOnlyWithDelimiters)
{
    std::vector<std::string> lines = {"", ""};
    EXPECT_EQ(lines, SplitStringForLines("\n"));
}

TEST(ExtractDigitsFromEntity, OneDigitExtraction)
{
    std::vector<std::string> digits = {" _ "
                                       "| |"
                                       "|_|"};

    EXPECT_EQ(digits, ExtractDigitsFromEntity(" _ \n"
                                              "| |\n"
                                              "|_|\n"));
}

TEST(ExtractDigitsFromEntity, TwoDigitsExtraction)
{
    std::vector<std::string> digits = {" _ "
                                       "| |"
                                       "|_|",
                                       "   "
                                       "  |"
                                       "  |"};

    EXPECT_EQ(digits, ExtractDigitsFromEntity(" _    \n"
                                              "| |  |\n"
                                              "|_|  |\n"));
}

TEST(ExtractDigitsFromEntity, ThreeDigitsExtraction)
{
    std::vector<std::string> digits = {" _ "
                                       "| |"
                                       "|_|",
                                       "   "
                                       "  |"
                                       "  |",
                                       " _ "
                                       " _|"
                                       "|_ ",};

    EXPECT_EQ(digits, ExtractDigitsFromEntity(" _     _ \n"
                                              "| |  | _|\n"
                                              "|_|  ||_ \n"));
}
