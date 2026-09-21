#ifndef CODE_COMPARATOR_H
#define CODE_COMPARATOR_H

#include "CodeAnalyzer.h"
#include <string>

struct ComparisonResult {

    int healthScoreBefore = 0;
    int healthScoreAfter = 0;
    int healthImprovement = 0;

    int warningCountBefore = 0;
    int warningCountAfter = 0;
    int warningReduction = 0;

    int totalLinesBefore = 0;
    int totalLinesAfter = 0;

    int loopsBefore = 0;
    int loopsAfter = 0;

    int conditionsBefore = 0;
    int conditionsAfter = 0;

    int nestedLoopsBefore = 0;
    int nestedLoopsAfter = 0;

    std::string complexityBefore;
    std::string complexityAfter;
};

class CodeComparator {

public:

    ComparisonResult compare(
        const CodeMetrics& before,
        const CodeMetrics& after
    );
};

#endif