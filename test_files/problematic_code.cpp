#include <iostream>
using namespace std;

class Test {
public:

    void run() {

        for (int i = 0; i < 10; i++) {

            for (int j = 0; j < 10; j++) {

                for (int k = 0; k < 10; k++) {

                    cout << i + j + k << endl;

                }
            }
        }

        int x = 10;

        if (x > 5) {
            cout << "A";
        }

        if (x > 7) {
            cout << "B";
        }

        if (x > 8) {
            cout << "C";
        }

        if (x > 9) {
            cout << "D";
        }

        if (x == 10) {
            cout << "E";
        }
    }
};

int main() {

    Test t;
    t.run();

    return 0;
}