#include "CodeAnalyzer.h"

#include <iostream>
#include <iomanip>

using namespace std;

void printReport(const CodeMetrics& metrics) {

    cout << "\n";
    cout << "╔══════════════════════════════════════════════╗\n";
    cout << "║              🩺 CODEMEDIC                   ║\n";
    cout << "║        C++ CODE HEALTH ANALYZER              ║\n";
    cout << "╠══════════════════════════════════════════════╣\n";

    cout << "║ CODE STATISTICS                              ║\n";
    cout << "║ Total Lines              : "
         << setw(4) << metrics.totalLines << "             ║\n";

    cout << "║ Code Lines               : "
         << setw(4) << metrics.codeLines << "             ║\n";

    cout << "║ Comment Lines            : "
         << setw(4) << metrics.commentLines << "             ║\n";

    cout << "║ Functions                : "
         << setw(4) << metrics.functions << "             ║\n";

    cout << "║ Classes                  : "
         << setw(4) << metrics.classes << "             ║\n";

    cout << "║ Loops                    : "
         << setw(4) << metrics.loops << "             ║\n";

    cout << "║ Conditions               : "
         << setw(4) << metrics.conditions << "             ║\n";

    cout << "║ Nested Loops             : "
         << setw(4) << metrics.nestedLoops << "             ║\n";

    cout << "╠══════════════════════════════════════════════╣\n";
    cout << "║ 🧬 OOP ANALYSIS                             ║\n";

    cout << "║ Encapsulation            : "
         << (metrics.encapsulationDetected ? "DETECTED" : "NOT DETECTED")
         << "             ║\n";

    cout << "║ Inheritance              : "
         << (metrics.inheritanceDetected ? "DETECTED" : "NOT DETECTED")
         << "             ║\n";

    cout << "║ Polymorphism             : "
         << (metrics.polymorphismDetected ? "DETECTED" : "NOT DETECTED")
         << "             ║\n";

    cout << "╠══════════════════════════════════════════════╣\n";
    cout << "║ 🧠 COMPLEXITY ANALYSIS                      ║\n";

    cout << "║ Estimated Complexity     : "
     << metrics.estimatedComplexity << "                 ║\n";

cout << "╠══════════════════════════════════════════════╣\n";
cout << "║ 🩺 CODE HEALTH                              ║\n";

cout << "║ Health Score              : "
     << metrics.healthScore << "/100"
     << "                  ║\n";

cout << "║ Health Status             : "
     << metrics.healthStatus
     << "              ║\n";

cout << "║ Warnings                  : "
     << metrics.warningCount
     << "                  ║\n";

cout << "╚══════════════════════════════════════════════╝\n";
}

int main() {

    cout << "\n==============================================\n";
    cout << "          🩺 CODEMEDIC 2.0\n";
    cout << "       C++ CODE HEALTH ANALYZER\n";
    cout << "==============================================\n";

    string filename;

    cout << "\nEnter C++ source file path: ";
    getline(cin, filename);

    CodeAnalyzer analyzer;

    if (!analyzer.loadFile(filename)) {
        cout << "\n❌ ERROR: Could not open the file.\n";
        cout << "Please check the file path and try again.\n";
        return 1;
    }

    cout << "\n🔍 Analyzing your code...\n";

    CodeMetrics metrics = analyzer.analyze();

    printReport(metrics);

    cout << "\n✅ Analysis completed successfully!\n";

    return 0;
}