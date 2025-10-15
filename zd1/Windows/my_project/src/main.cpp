#include <iostream>
#include <cmath>
#include "Number.h"
#include "Vector.h"

void printNumber(const std::string& name, const Number& num) {
    std::cout << name << " = " << num.getValue() << std::endl;
}

void printVector(const std::string& name, const Vector& vec) {
    std::cout << name << ": (" << vec.getX().getValue() << ", " << vec.getY().getValue() << ")" << std::endl;
}

int main() {
    std::cout << "--- Тестирование библиотеки Number (Статическая) ---" << std::endl;

    Number a = createNumber(5.0);
    Number b = createNumber(2.0);
    printNumber("a", a);
    printNumber("b", b);
    printNumber("ZERO", ZERO);
    printNumber("ONE", ONE);

    Number sum = a + b;
    Number diff = a - b;
    Number prod = a * b;
    Number quot = a / b;

    printNumber("a + b", sum);
    printNumber("a - b", diff);
    printNumber("a * b", prod);
    printNumber("a / b", quot);
    
    std::cout << "\n--- Тестирование библиотеки Vector (Динамическая) ---" << std::endl;

    Vector v1(createNumber(3.0), createNumber(4.0));
    printVector("v1", v1);
    printVector("VECTOR_ONE", VECTOR_ONE);

    Vector v_sum = v1.add(VECTOR_ONE);
    printVector("v1 + VECTOR_ONE", v_sum);

    Number r = v1.getPolarR();
    Number theta = v1.getPolarTheta();

    printNumber("v1 Polar R (длина)", r);
    printNumber("v1 Polar Theta (угол в рад)", theta);

    Vector v2(ONE, ZERO);
    Number r2 = v2.getPolarR();
    Number theta2 = v2.getPolarTheta();
    printNumber("v2 Polar Theta (угол)", theta2);

    return 0;
}
