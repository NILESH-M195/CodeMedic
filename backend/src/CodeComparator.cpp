#include "../include/CodeComparator.h"

ComparisonResult CodeComparator::compare(
    const CodeMetrics& before,
    const CodeMetrics& after
) {
    ComparisonResult result;

    result.healthScoreBefore = before.healthScore;
    result.healthScoreAfter = after.healthScore;
    result.healthImprovement =
        after.healthScore - before.healthScore;

    result.warningCountBefore = before.warningCount;
    result.warningCountAfter = after.warningCount;
    result.warningReduction =
        before.warningCount - after.warningCount;

    result.totalLinesBefore = before.totalLines;
    result.totalLinesAfter = after.totalLines;

    result.loopsBefore = before.loops;
    result.loopsAfter = after.loops;

    result.conditionsBefore = before.conditions;
    result.conditionsAfter = after.conditions;

    result.nestedLoopsBefore = before.nestedLoops;
    result.nestedLoopsAfter = after.nestedLoops;

    result.complexityBefore = before.estimatedComplexity;
    result.complexityAfter = after.estimatedComplexity;

    return result;
}