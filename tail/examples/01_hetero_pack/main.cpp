/**
 * This code is a story.
 */

#include <ecosnail/containers.hpp>

#include <utility>
#include <string>
#include <iostream>

namespace ec = ecosnail::containers;

template <class T, class Tag>
class TaggedType {
public:
    explicit TaggedType(T value) : _value(std::move(value)) {}

    operator const T&() const { return _value; }
    bool operator==(const TaggedType<T, Tag>& other) const { return _value == other._value; }

    friend std::ostream& operator<<(std::ostream& os, const TaggedType& tt)
    {
        os << tt._value;
        return os;
    }

private:
    T _value;
};

struct PersonNameTag {};
typedef TaggedType<std::string, PersonNameTag> PersonName;

struct PersonAgeTag {};
typedef TaggedType<double, PersonAgeTag> PersonAge;

struct PersonHeightTag {};
typedef TaggedType<double, PersonHeightTag> PersonHeight;

struct PersonYearSalaryTag {};
typedef TaggedType<double, PersonYearSalaryTag> PersonYearSalary;

typedef ec::HeteroPack<
    PersonName,
    PersonAge,
    PersonHeight,
    PersonYearSalary
> Person;

void printPersonInfo(const Person& person)
{
    std::cout << person.at<PersonName>() << ":" << std::endl <<
        "  Age    : " << person.at<PersonAge>() << std::endl <<
        "  Height : " << person.at<PersonHeight>() << std::endl <<
        "  Salary : " << person.at<PersonYearSalary>() << std::endl <<
        std::endl;
}

int main()
{
    Person someone {
        PersonName{"John"},
        PersonAge{18},
        PersonHeight{180},
        PersonYearSalary{1234},
    };

    Person someoneElse {
        PersonHeight{191.5},
        PersonName{"Jack"},
        PersonYearSalary{4321.5},
        PersonAge{30},
    };

    Person someoneElseAgain {
        PersonYearSalary{4321.5},
        PersonName{"Jack"},
        PersonHeight{191.5},
        PersonAge{30},
    };

    printPersonInfo(someone);
    printPersonInfo(someoneElse);
    printPersonInfo(someoneElseAgain);

    std::cout << std::boolalpha;
    std::cout <<
        "someone     = someoneElse      : " <<
            (someone == someoneElse) << std::endl <<
        "someoneElse = someoneElseAgain : " <<
            (someoneElse == someoneElseAgain) << std::endl;
}