let beforeAnalysis = null;
let afterAnalysis = null;
const fileInput = document.getElementById("fileInput");
const selectedFile = document.getElementById("selectedFile");
const analyzeButton = document.getElementById("analyzeButton");
const uploadCard = document.querySelector(".upload-card");

let selectedCppFile = null;


// ==========================================
// FILE HANDLING
// ==========================================

function handleFile(file) {

    if (!file) {
        return;
    }

    if (!file.name.toLowerCase().endsWith(".cpp")) {

        selectedCppFile = null;

        selectedFile.textContent =
            "Please select a valid .cpp file.";

        return;
    }

    selectedCppFile = file;

selectedFile.textContent =
    "Selected: " + file.name;

// Reset analyze button for the new file
analyzeButton.innerHTML =
    "Analyze Code <span>→</span>";

analyzeButton.disabled = false;
}


// Choose file
fileInput.addEventListener("change", function () {

    if (fileInput.files.length > 0) {

        handleFile(fileInput.files[0]);

    }

});


// ==========================================
// DRAG & DROP
// ==========================================

uploadCard.addEventListener("dragover", function (event) {

    event.preventDefault();

    uploadCard.style.borderColor =
        "#4f8cff";

});


uploadCard.addEventListener("dragleave", function () {

    uploadCard.style.borderColor =
        "rgba(148, 163, 184, 0.16)";

});


uploadCard.addEventListener("drop", function (event) {

    event.preventDefault();

    uploadCard.style.borderColor =
        "rgba(148, 163, 184, 0.16)";

    const files = event.dataTransfer.files;

    if (files.length > 0) {

        handleFile(files[0]);

    }

});


// ==========================================
// ANALYZE CODE
// ==========================================

analyzeButton.addEventListener("click", async function () {

    if (!selectedCppFile) {

        selectedFile.textContent =
            "Please choose a .cpp file first.";

        return;
    }


    analyzeButton.innerHTML =
        "Analyzing... <span>◌</span>";

    analyzeButton.disabled = true;


    try {

        // Read the C++ source file
        const code =
            await selectedCppFile.text();


        // Send source code to C++ backend
        const response =
            await fetch(
                "http://127.0.0.1:8080/analyze",
                {
                    method: "POST",

                    headers: {
                        "Content-Type": "text/plain"
                    },

                    body: code
                }
            );


        // Convert backend response to JSON
        const result =
            await response.json();


        // Check for backend error
        if (result.error) {

            throw new Error(result.error);

        }
        // Store BEFORE and AFTER analysis
if (beforeAnalysis === null) {
    beforeAnalysis = result;

    selectedFile.textContent =
        "Before version saved. Now select the improved .cpp file and analyze it.";

    console.log("BEFORE analysis saved:", beforeAnalysis);

} else {
    afterAnalysis = result;

    const comparison =
        compareAnalyses(beforeAnalysis, afterAnalysis);

    console.log("FINAL COMPARISON:", comparison);

    selectedFile.textContent =
        "Before vs After comparison completed.";
}


        // Update dashboard
        updateDashboard(result);


        analyzeButton.innerHTML =
            "Analysis Complete <span>✓</span>";


    }
    catch (error) {

        console.error(error);

        selectedFile.textContent =
            "Analysis failed: " + error.message;

        analyzeButton.innerHTML =
            "Analysis Failed <span>!</span>";

    }


    analyzeButton.disabled = false;

});


// ==========================================
// UPDATE DASHBOARD
// ==========================================

function updateDashboard(data) {

    console.log("CodeMedic analysis:", data);
    if (!beforeAnalysis) {
    beforeAnalysis = data;
    console.log("Before code analysis saved.");
} else {
    afterAnalysis = data;
    console.log("After code analysis saved.");
    compareAnalyses(beforeAnalysis, afterAnalysis);
}


    // --------------------------------------
    // Health Score
    // --------------------------------------

    const metricValues =
        document.querySelectorAll(".metric-value");


    if (metricValues.length >= 4) {

        metricValues[0].innerHTML =
            `${data.healthScore}<span>/100</span>`;

        metricValues[1].textContent =
            data.estimatedComplexity;

        metricValues[2].textContent =
            data.warningCount;

        metricValues[3].textContent =
            data.totalLines;
    }
const healthScoreStatus =
    document.querySelector(".metric-card .metric-description");

if (healthScoreStatus) {

    healthScoreStatus.textContent =
        data.healthStatus;
}

    // --------------------------------------
    // File name
    // --------------------------------------

    const fileBadge =
        document.querySelector(".file-badge");

    if (fileBadge) {

        fileBadge.textContent =
            selectedCppFile.name;
    }


    // --------------------------------------
    // Health ring
    // --------------------------------------

    const healthRing =
        document.querySelector(".health-ring");

    if (healthRing) {

        const degrees =
            data.healthScore * 3.6;

        healthRing.style.background =
            `conic-gradient(
                #4f8cff 0deg ${degrees}deg,
                rgba(255,255,255,0.06)
                ${degrees}deg 360deg
            )`;
    }


    // --------------------------------------
    // Health ring number
    // --------------------------------------

    const healthNumber =
        document.querySelector(
            ".health-ring-inner strong"
        );

    if (healthNumber) {

        healthNumber.textContent =
            data.healthScore;
    }


    // --------------------------------------
    // Health status
    // --------------------------------------

    const panelStatus =
        document.querySelector(".panel-status");

    if (panelStatus) {

        panelStatus.textContent =
            data.healthStatus;
    }


    // --------------------------------------
    // Health description
    // --------------------------------------

    const healthDescription =
        document.querySelector(".health-details p");

    if (healthDescription) {

        healthDescription.textContent =
            getHealthMessage(data.healthScore);
    }


    // --------------------------------------
    // Health statistics
    // --------------------------------------

    const healthStats =
        document.querySelectorAll(
            ".health-stat strong"
        );


    if (healthStats.length >= 3) {

        healthStats[0].textContent =
            data.warningCount;

        healthStats[1].textContent =
            data.nestedLoops;

        healthStats[2].textContent =
            data.conditions;
    }


    // --------------------------------------
    // Complexity
    // --------------------------------------

    const complexitySymbol =
        document.querySelector(
            ".complexity-symbol"
        );

    if (complexitySymbol) {

        complexitySymbol.textContent =
            data.estimatedComplexity;
    }


    // --------------------------------------
    // Complexity text
    // --------------------------------------

    const complexityDescription =
        document.querySelector(
            ".complexity-main p"
        );

    if (complexityDescription) {

        complexityDescription.textContent =
            getComplexityMessage(
                data.estimatedComplexity
            );
    }


    // --------------------------------------
    // Nested loops
    // --------------------------------------

    const complexityFooter =
        document.querySelector(
            ".complexity-footer strong"
        );

    if (complexityFooter) {

        complexityFooter.textContent =
            `${data.nestedLoops} detected`;
    }


    // --------------------------------------
    // Source statistics
    // --------------------------------------

    const statValues =
        document.querySelectorAll(
            ".stat-row strong"
        );


    if (statValues.length >= 6) {

        statValues[0].textContent =
            data.totalLines;

        statValues[1].textContent =
            data.codeLines;

        statValues[2].textContent =
            data.commentLines;

        statValues[3].textContent =
            data.functions;

        statValues[4].textContent =
            data.loops;

        statValues[5].textContent =
            data.conditions;
    }


    // --------------------------------------
    // OOP analysis
    // --------------------------------------

    const oopNumber =
        document.querySelector(
            ".oop-number"
        );

    if (oopNumber) {

        oopNumber.textContent =
            data.classes;
    }


    const badges =
        document.querySelectorAll(
            ".badge"
        );


    if (badges.length >= 3) {

        updateBadge(
            badges[0],
            data.encapsulationDetected
        );

        updateBadge(
            badges[1],
            data.inheritanceDetected
        );

        updateBadge(
            badges[2],
            data.polymorphismDetected
        );
    }


    // --------------------------------------
    // Warning count
    // --------------------------------------

    const warningCount =
        document.querySelector(
            ".warning-count"
        );

    if (warningCount) {

        warningCount.textContent =
            `${data.warningCount} warning${
                data.warningCount === 1 ? "" : "s"
            }`;
    }

}


// ==========================================
// BADGE UPDATE
// ==========================================

function updateBadge(element, detected) {

    if (detected) {

        element.textContent =
            "DETECTED";

        element.className =
            "badge success";

    }
    else {

        element.textContent =
            "NOT DETECTED";

        element.className =
            "badge neutral";
    }
}


// ==========================================
// HEALTH MESSAGE
// ==========================================

function getHealthMessage(score) {

    if (score >= 90) {

        return "Your code shows strong overall health and structure.";

    }

    if (score >= 75) {

        return "Your code is generally healthy with a few areas for improvement.";

    }

    if (score >= 50) {

        return "Your code is functional but has areas that could be improved for maintainability and complexity.";

    }

    return "Your code requires significant attention to improve quality and maintainability.";
}


// ==========================================
// COMPLEXITY MESSAGE
// ==========================================

function getComplexityMessage(complexity) {

    if (complexity === "O(1)") {

        return "Constant-time behavior detected.";

    }

    if (complexity === "O(n)") {

        return "Linear iteration detected in the source code.";

    }

    if (
        complexity === "O(n^2)" ||
        complexity === "O(n²)"
    ) {

        return "Nested iteration detected in the source code.";

    }

    return "Complexity estimated from the detected control structures.";
}
// ==================== BEFORE vs AFTER COMPARISON ====================
function compareAnalyses(before, after) {

    const healthChange =
        after.healthScore - before.healthScore;

    const warningChange =
        before.warningCount - after.warningCount;

    const complexityChanged =
        before.estimatedComplexity !==
        after.estimatedComplexity;

    console.log("===== CODEMEDIC COMPARISON =====");
    console.log("Health change:", healthChange);
    console.log("Warnings reduced:", warningChange);
    console.log("Complexity changed:", complexityChanged);

    // Update comparison cards
    const beforeHealth =
        document.getElementById("beforeHealth");

    const afterHealth =
        document.getElementById("afterHealth");

    const healthChangeElement =
        document.getElementById("healthChange");

    const beforeWarnings =
        document.getElementById("beforeWarnings");

    const afterWarnings =
        document.getElementById("afterWarnings");

    const warningChangeElement =
        document.getElementById("warningChange");

    const beforeComplexity =
        document.getElementById("beforeComplexity");

    const afterComplexity =
        document.getElementById("afterComplexity");

    const complexityChangeElement =
        document.getElementById("complexityChange");

    if (beforeHealth)
        beforeHealth.textContent =
            before.healthScore + "/100";

    if (afterHealth)
        afterHealth.textContent =
            after.healthScore + "/100";

    if (healthChangeElement)
        healthChangeElement.textContent =
            (healthChange >= 0 ? "+" : "") +
            healthChange +
            " health points";

    if (beforeWarnings)
        beforeWarnings.textContent =
            before.warningCount;

    if (afterWarnings)
        afterWarnings.textContent =
            after.warningCount;

    if (warningChangeElement)
        warningChangeElement.textContent =
            warningChange >= 0
                ? warningChange + " warnings reduced"
                : Math.abs(warningChange) +
                  " additional warnings";

    if (beforeComplexity)
        beforeComplexity.textContent =
            before.estimatedComplexity;

    if (afterComplexity)
        afterComplexity.textContent =
            after.estimatedComplexity;

    if (complexityChangeElement)
        complexityChangeElement.textContent =
            complexityChanged
                ? "Complexity improved"
                : "No complexity change";
}

