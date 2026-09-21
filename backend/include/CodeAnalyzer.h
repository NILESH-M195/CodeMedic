#ifndef CODE_ANALYZER_H
#define CODE_ANALYZER_H

#include <string>
#include <vector>

struct CodeMetrics {
    int totalLines = 0;
    int codeLines = 0;
    int commentLines = 0;
    int functions = 0;
    int loops = 0;
    int conditions = 0;
    int classes = 0;
    int nestedLoops = 0;

    bool inheritanceDetected = false;
    bool polymorphismDetected = false;
    bool encapsulationDetected = false;

    std::string estimatedComplexity = "O(1)";

    // Code Health
    int healthScore = 100;
    std::string healthStatus = "EXCELLENT";

    // Detected issues
    int warningCount = 0;
    // Diagnosis messages
std::vector<std::string> warnings;
};
class CodeAnalyzer {
private:
    std::vector<std::string> lines;

    bool contains(const std::string& line, const std::string& pattern);

    int countFunctions();
    int countLoops();
    int countConditions();
    int countClasses();
    int detectNestedLoops();

    void analyzeOOP(CodeMetrics& metrics);
    void estimateComplexity(CodeMetrics& metrics);

public:
    bool loadFile(const std::string& filename);
    CodeMetrics analyze();

    void calculateHealth(CodeMetrics& metrics);
    void generateWarnings(CodeMetrics& metrics);
};

#endif