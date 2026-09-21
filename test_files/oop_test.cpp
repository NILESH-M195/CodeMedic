#include <iostream>
using namespace std;

class Animal {
private:
    int age;

public:
    Animal(int a) {
        age = a;
    }

    virtual void sound() {
        cout << "Animal sound";
    }
};

class Dog : public Animal {
public:
    Dog(int a) : Animal(a) {}

    void sound() override {
        cout << "Dog barks";
    }
};

int main() {

    Dog d(3);
    d.sound();

    return 0;
}