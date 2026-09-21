#include <iostream>
using namespace std;

class Student {
private:
    int marks;

public:
    Student(int m) {
        marks = m;
    }

    void display() {
        cout << "Marks: " << marks << endl;
    }
};

int main() {
    Student s(85);
    s.display();

    for (int i = 0; i < 5; i++) {
        cout << i << endl;
    }

    return 0;
}
