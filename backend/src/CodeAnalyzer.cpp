#include "CodeAnalyzer.h"

#include <fstream>
#include <algorithm>
#include <cctype>

using namespace std;

bool CodeAnalyzer::loadFile(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        return false;
    }

    lines.clear();

    string line;

    while (getline(file, line)) {
        lines.push_back(line);
    }

    file.close();

    return true;
}

bool CodeAnalyzer::contains(const string& line, const string& pattern) {
    return line.find(pattern) != string::npos;
}

int CodeAnalyzer::countFunctions() {
    int count = 0;

    for (const string& line : lines) {

        if (contains(line, "(") &&
            contains(line, ")") &&
            contains(line, "{")) {

            if (!contains(line, "if") &&
                !contains(line, "for") &&
                !contains(line, "while") &&
                !contains(line, "switch") &&
                !contains(line, "catch")) {

                count++;
            }
        }
    }

    return count;
}

int CodeAnalyzer::countLoops() {
    int count = 0;

    for (const string& line : lines) {

        if (contains(line, "for") ||
            contains(line, "while") ||
            contains(line, "do")) {

            count++;
        }
    }

    return count;
}

int CodeAnalyzer::countConditions() {
    int count = 0;

    for (const string& line : lines) {

        if (contains(line, "if") ||
            contains(line, "else if") ||
            contains(line, "switch") ||
            contains(line, "?")) {

            count++;
        }
    }

    return count;
}

int CodeAnalyzer::countClasses() {
    int count = 0;

    for (const string& line : lines) {

        string trimmed = line;

        trimmed.erase(
            trimmed.begin(),
            find_if(
                trimmed.begin(),
                trimmed.end(),
                [](unsigned char ch) {
                    return !isspace(ch);
                }
            )
        );

        if (trimmed.rfind("class ", 0) == 0) {
            count++;
        }
    }

    return count;
}

int CodeAnalyzer::detectNestedLoops() {
    int nested = 0;
    int loopDepth = 0;

    for (const string& line : lines) {

        if (contains(line, "for") ||
            contains(line, "while")) {

            if (loopDepth > 0) {
                nested++;
            }

            loopDepth++;
        }

        int closingBraces =
            count(line.begin(), line.end(), '}');

        loopDepth -= closingBraces;

        if (loopDepth < 0) {
            loopDepth = 0;
        }
    }

    return nested;
}

void CodeAnalyzer::analyzeOOP(CodeMetrics& metrics) {

    for (const string& line : lines) {

        

        if (contains(line, "public:") ||
            contains(line, "private:") ||
            contains(line, "protected:")) {

            metrics.encapsulationDetected = true;
        }

        if (contains(line, " : public ") ||
            contains(line, " : protected ") ||
            contains(line, " : private ")) {

            metrics.inheritanceDetected = true;
        }

        if (contains(line, "virtual ")) {
            metrics.polymorphismDetected = true;
        }
    }
}

void CodeAnalyzer::estimateComplexity(CodeMetrics& metrics) {

    if (metrics.nestedLoops >= 3) {
        metrics.estimatedComplexity = "O(n^4)";
    }
    else if (metrics.nestedLoops == 2) {
        metrics.estimatedComplexity = "O(n^3)";
    }
    else if (metrics.nestedLoops == 1) {
        metrics.estimatedComplexity = "O(n^2)";
    }
    else if (metrics.loops > 0) {
        metrics.estimatedComplexity = "O(n)";
    }
    else {
        metrics.estimatedComplexity = "O(1)";
    }
}

CodeMetrics CodeAnalyzer::analyze() {

    CodeMetrics metrics;

    metrics.totalLines = static_cast<int>(lines.size());

    for (const string& line : lines) {

        string trimmed = line;

        trimmed.erase(
            trimmed.begin(),
            find_if(
                trimmed.begin(),
                trimmed.end(),
                [](unsigned char ch) {
                    return !isspace(ch);
                }
            )
        );

        if (trimmed.empty()) {
            continue;
        }

        if (trimmed.rfind("//", 0) == 0) {
            metrics.commentLines++;
        }
        else {
            metrics.codeLines++;
        }
    }

    metrics.functions = countFunctions();
    metrics.loops = countLoops();
    metrics.conditions = countConditions();
    metrics.classes = countClasses();
    metrics.nestedLoops = detectNestedLoops();

        analyzeOOP(metrics);
    estimateComplexity(metrics);
    calculateHealth(metrics);
    generateWarnings(metrics);

    return metrics;
}

void CodeAnalyzer::calculateHealth(CodeMetrics& metrics) {

    int score = 100;

    // Complexity penalty
    if (metrics.nestedLoops >= 1)
        score -= 20;

    if (metrics.nestedLoops >= 2)
        score -= 10;

    // Too many conditions
    if (metrics.conditions >= 5)
        score -= 10;

    // Too many loops
    if (metrics.loops >= 5)
        score -= 10;

    // Large source file
    if (metrics.totalLines > 300)
        score -= 10;

    // Lack of comments
    if (metrics.totalLines > 20 && metrics.commentLines == 0)
        score -= 10;

    // OOP encapsulation
    if (metrics.classes > 0 && !metrics.encapsulationDetected)
        score -= 10;

    if (score < 0)
        score = 0;

    metrics.healthScore = score;

    if (score >= 90)
        metrics.healthStatus = "EXCELLENT";
    else if (score >= 75)
        metrics.healthStatus = "HEALTHY";
    else if (score >= 50)
        metrics.healthStatus = "NEEDS ATTENTION";
    else
        metrics.healthStatus = "CRITICAL";

    metrics.warningCount = 0;

    if (metrics.nestedLoops > 0)
        metrics.warningCount++;

    if (metrics.conditions >= 5)
        metrics.warningCount++;

    if (metrics.loops >= 5)
        metrics.warningCount++;

    if (metrics.totalLines > 300)
        metrics.warningCount++;

    if (metrics.totalLines > 20 && metrics.commentLines == 0)
        metrics.warningCount++;
}
void CodeAnalyzer::generateWarnings(CodeMetrics& metrics) {

    metrics.warnings.clear();

    // Nested loops
    if (metrics.nestedLoops > 0) {
        metrics.warnings.push_back(
            "Nested loops detected. Consider reducing nested iteration to improve time efficiency."
        );
    }

    // Too many conditions
    if (metrics.conditions >= 5) {
        metrics.warnings.push_back(
            "Many conditional statements detected. Consider simplifying complex decision logic."
        );
    }

    // Too many loops
    if (metrics.loops >= 5) {
        metrics.warnings.push_back(
            "High loop count detected. Review repeated operations for possible optimization."
        );
    }

    // Large source file
    if (metrics.totalLines > 300) {
        metrics.warnings.push_back(
            "Large source file detected. Consider splitting the code into smaller modules."
        );
    }

    // Missing comments
    if (metrics.totalLines > 20 && metrics.commentLines == 0) {
        metrics.warnings.push_back(
            "No comments detected. Add comments to explain important logic and improve maintainability."
        );
    }

    // Encapsulation
    if (metrics.classes > 0 &&
        !metrics.encapsulationDetected) {

        metrics.warnings.push_back(
            "Class detected without clear encapsulation. Consider using private and protected members."
        );
    }

    metrics.warningCount =
        static_cast<int>(metrics.warnings.size());
}