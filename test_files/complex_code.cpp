#include <iostream>
using namespace std;

class DataProcessor {
public:
    void processData() {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                cout << i * j << " ";
            }
            cout << endl;
        }

        int x = 10;

        if (x > 5) {
            cout << "Large value" << endl;
        }

        if (x == 10) {
            cout << "Value is ten" << endl;
        }
    }
};

int main() {
    DataProcessor processor;
    processor.processData();

    for (int i = 0; i < 20; i++) {
        cout << i << endl;
    }

    return 0;
}
