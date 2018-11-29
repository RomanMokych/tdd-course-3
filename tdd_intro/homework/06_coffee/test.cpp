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

    void SetupCappuchinoExpectations(int grams)
    {
        const int cappuccinoTemperature = 80;

        EXPECT_CALL(*this, AddWater(grams, cappuccinoTemperature)).Times(1);
        EXPECT_CALL(*this, AddMilk(grams / 3)).Times(1);
        EXPECT_CALL(*this, AddCoffee(grams / 3)).Times(1);
        EXPECT_CALL(*this, AddMilkFoam(grams / 3)).Times(1);
    }

    void SetupLatteExpectations(int grams)
    {
        const int temperature = 90;

        EXPECT_CALL(*this, AddWater(grams, temperature)).Times(1);
        EXPECT_CALL(*this, AddMilk(grams / 4)).Times(1);
        EXPECT_CALL(*this, AddCoffee(grams / 2)).Times(1);
        EXPECT_CALL(*this, AddMilkFoam(grams / 4)).Times(1);
    }
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
        const int grams = GramsFromCupSize(cupSize);
        const int temperature = 80;

        m_ingridientsSource.AddWater(grams, temperature);
        m_ingridientsSource.AddMilk(grams / 3);
        m_ingridientsSource.AddCoffee(grams / 3);
        m_ingridientsSource.AddMilkFoam(grams / 3);
    }

    void MakeLatte(const CupSize cupSize)
    {
        const int grams = GramsFromCupSize(cupSize);
        const int temperature = 90;

        m_ingridientsSource.AddWater(grams, temperature);
        m_ingridientsSource.AddMilk(grams / 4);
        m_ingridientsSource.AddCoffee(grams / 2);
        m_ingridientsSource.AddMilkFoam(grams / 4);
    }

private:
    int GramsFromCupSize(const CupSize cupSize)
    {
        if (cupSize == CupSize::Little)
        {
            return 100;
        }

        return 140; //for CupSize::Big
    }

private:
    ISourceOfIngredients& m_ingridientsSource;
};

TEST(CoffeeMachine, MakeBigCappuccino)
{
    MockSourceOfIngredients ingridientsSource;

    const int grams = 140;
    ingridientsSource.SetupCappuchinoExpectations(grams);

    CoffeeMachine coffeeMachine(ingridientsSource);
    coffeeMachine.MakeCappuccino(CupSize::Big);
}

TEST(CoffeeMachine, MakeLittleCappuccino)
{
    MockSourceOfIngredients ingridientsSource;

    const int grams = 100;
    ingridientsSource.SetupCappuchinoExpectations(grams);

    CoffeeMachine coffeeMachine(ingridientsSource);
    coffeeMachine.MakeCappuccino(CupSize::Little);
}

TEST(CoffeeMachine, MakeBigLatte)
{
    MockSourceOfIngredients ingridientsSource;

    const int grams = 140;
    ingridientsSource.SetupLatteExpectations(grams);

    CoffeeMachine coffeeMachine(ingridientsSource);
    coffeeMachine.MakeLatte(CupSize::Big);
}
