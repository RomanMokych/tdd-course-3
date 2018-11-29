/*
We have to develop a coffee machine with TDD. The machine uses the automated source of ingredients, which implements the interface ISourceOfIngredients.

Our machine should be able to produce the coffee accordingly to the next receipts:
- americano: water & coffee 1:2 or 1:3. Water temp 60C
- cappuccino - milk & coffee & milk foam 1:3, 1:3, 1:3. Water temp 80C
- latte - milk & coffee & milk foam 1:4, 1:2, 1:4. Water temp 90C
- marochino - chocolate & coffee & milk foam, 1:4, 1:4, 1:4 and 1:4 is empty

We have 2 possible sizes of the cup:
- little 100 gram
- big 140 gram

Implement worked coffee machine using ISourceOfIngredients to controll the process of coffee production.
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class ISourceOfIngredients
{
public:
    virtual ~ISourceOfIngredients() {}
    virtual void SetCupSize(int gram) = 0;
    virtual void AddWater(int gram, int temperature) = 0;
    virtual void AddSugar(int gram) = 0;
    virtual void AddCoffee(int gram) = 0;
    virtual void AddMilk(int gram) = 0;
    virtual void AddMilkFoam(int gram) = 0;
    virtual void AddChocolate(int gram) = 0;
    virtual void AddCream(int gram) = 0;
};

class MockSourceOfIngredients : public ISourceOfIngredients
{
public:
    MOCK_METHOD1(SetCupSize, void(int));
    MOCK_METHOD2(AddWater, void(int, int));
    MOCK_METHOD1(AddSugar, void(int));
    MOCK_METHOD1(AddCoffee, void(int));
    MOCK_METHOD1(AddMilk, void(int));
    MOCK_METHOD1(AddMilkFoam, void(int));
    MOCK_METHOD1(AddChocolate, void(int));
    MOCK_METHOD1(AddCream, void(int));
};

enum CupSize
{
    Little,
    Big
};

class CoffeeMachine
{
public:
    CoffeeMachine(ISourceOfIngredients& source) : m_ingridientsSource(source)
    {}

    void MakeCappuccino(const CupSize cupSize)
    {
        m_ingridientsSource.AddWater(140, 80);
        m_ingridientsSource.AddMilk(140 / 3);
        m_ingridientsSource.AddCoffee(140 / 3);
        m_ingridientsSource.AddMilkFoam(140 / 3);
    }

private:
    ISourceOfIngredients& m_ingridientsSource;
};

TEST(CoffeeMachine, MakeBigCappuccino)
{
    MockSourceOfIngredients ingridientsSource;

    EXPECT_CALL(ingridientsSource, AddWater(140, 80)).Times(1);
    EXPECT_CALL(ingridientsSource, AddMilk(140 / 3)).Times(1);
    EXPECT_CALL(ingridientsSource, AddCoffee(140 / 3)).Times(1);
    EXPECT_CALL(ingridientsSource, AddMilkFoam(140 / 3)).Times(1);

    CoffeeMachine coffeeMachine(ingridientsSource);

    coffeeMachine.MakeCappuccino(CupSize::Big);
}
