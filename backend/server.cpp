#include "CodeAnalyzer.h"
#include "codeComparator.h"
#include <winsock2.h>
#include <windows.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;


// Send an HTTP response to the browser
void sendResponse(
    SOCKET client,
    const string& body,
    const string& contentType = "application/json"
) {

    string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: " + contentType + "\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: POST, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type\r\n"
        "Content-Length: " + to_string(body.size()) + "\r\n"
        "Connection: close\r\n"
        "\r\n" +
        body;

    send(
        client,
        response.c_str(),
        static_cast<int>(response.size()),
        0
    );
}


// Create JSON from analysis results
string createJSON(const CodeMetrics& metrics) {

    stringstream json;

    json << "{";

    json << "\"totalLines\":" << metrics.totalLines << ",";
    json << "\"codeLines\":" << metrics.codeLines << ",";
    json << "\"commentLines\":" << metrics.commentLines << ",";
    json << "\"functions\":" << metrics.functions << ",";
    json << "\"loops\":" << metrics.loops << ",";
    json << "\"conditions\":" << metrics.conditions << ",";
    json << "\"classes\":" << metrics.classes << ",";
    json << "\"nestedLoops\":" << metrics.nestedLoops << ",";

    json << "\"inheritanceDetected\":"
         << (metrics.inheritanceDetected ? "true" : "false") << ",";

    json << "\"polymorphismDetected\":"
         << (metrics.polymorphismDetected ? "true" : "false") << ",";

    json << "\"encapsulationDetected\":"
         << (metrics.encapsulationDetected ? "true" : "false") << ",";

    json << "\"estimatedComplexity\":\""
         << metrics.estimatedComplexity << "\",";

    json << "\"healthScore\":"
         << metrics.healthScore << ",";

    json << "\"healthStatus\":\""
         << metrics.healthStatus << "\",";
        // Generate improvement recommendations
    json << "\"recommendations\":[";

    bool firstRecommendation = true;

    if (metrics.nestedLoops > 0) {
        json << "\"Reduce nested loops to improve performance and scalability.\"";
        firstRecommendation = false;
    }

    if (metrics.conditions >= 5) {
        if (!firstRecommendation) json << ",";
        json << "\"Simplify complex conditional logic where possible.\"";
        firstRecommendation = false;
    }

    if (metrics.loops >= 5) {
        if (!firstRecommendation) json << ",";
        json << "\"Consider reducing repeated loops or extracting reusable logic.\"";
        firstRecommendation = false;
    }

    if (metrics.commentLines == 0 && metrics.totalLines > 20) {
        if (!firstRecommendation) json << ",";
        json << "\"Add meaningful comments to improve code maintainability.\"";
        firstRecommendation = false;
    }

    if (firstRecommendation) {
        json << "\"Code structure looks healthy. Keep following clean coding practices.\"";
    }

    json << "],";

    json << "\"warningCount\":"
         << metrics.warningCount;

    json << "}";

    return json.str();
}
// Create JSON from comparison results
string createComparisonJSON(const ComparisonResult& result) {

    stringstream json;

    json << "{";

    json << "\"healthScoreBefore\":"
         << result.healthScoreBefore << ",";

    json << "\"healthScoreAfter\":"
         << result.healthScoreAfter << ",";

    json << "\"healthImprovement\":"
         << result.healthImprovement << ",";

    json << "\"warningCountBefore\":"
         << result.warningCountBefore << ",";

    json << "\"warningCountAfter\":"
         << result.warningCountAfter << ",";

    json << "\"warningReduction\":"
         << result.warningReduction << ",";

    json << "\"totalLinesBefore\":"
         << result.totalLinesBefore << ",";

    json << "\"totalLinesAfter\":"
         << result.totalLinesAfter << ",";

    json << "\"loopsBefore\":"
         << result.loopsBefore << ",";

    json << "\"loopsAfter\":"
         << result.loopsAfter << ",";

    json << "\"conditionsBefore\":"
         << result.conditionsBefore << ",";

    json << "\"conditionsAfter\":"
         << result.conditionsAfter << ",";

    json << "\"nestedLoopsBefore\":"
         << result.nestedLoopsBefore << ",";

    json << "\"nestedLoopsAfter\":"
         << result.nestedLoopsAfter << ",";

    json << "\"complexityBefore\":\""
         << result.complexityBefore << "\",";

    json << "\"complexityAfter\":\""
         << result.complexityAfter << "\"";

    json << "}";

    return json.str();
}

// Get request body
string getRequestBody(const string& request) {

    size_t separator = request.find("\r\n\r\n");

    if (separator == string::npos) {
        return "";
    }

    return request.substr(separator + 4);
}


int main() {
    CodeComparator comparator;

    cout << "\n============================================\n";
    cout << "             CODEMEDIC SERVER\n";
    cout << "============================================\n";

    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {

        cout << "ERROR: Could not initialize Windows networking.\n";

        return 1;
    }


    SOCKET serverSocket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );

    if (serverSocket == INVALID_SOCKET) {

        cout << "ERROR: Could not create server socket.\n";

        WSACleanup();

        return 1;
    }


    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(8080);


    if (bind(
        serverSocket,
        reinterpret_cast<sockaddr*>(&serverAddress),
        sizeof(serverAddress)
    ) == SOCKET_ERROR) {

        cout << "ERROR: Could not bind to port 8080.\n";

        closesocket(serverSocket);
        WSACleanup();

        return 1;
    }


    if (listen(serverSocket, 5) == SOCKET_ERROR) {

        cout << "ERROR: Could not start listening.\n";

        closesocket(serverSocket);
        WSACleanup();

        return 1;
    }


    cout << "\nServer started successfully!\n";
    cout << "API running at:\n";
    cout << "http://127.0.0.1:8080\n";
    cout << "\nWaiting for CodeMedic frontend...\n";


    while (true) {

        SOCKET clientSocket = accept(
            serverSocket,
            nullptr,
            nullptr
        );


        if (clientSocket == INVALID_SOCKET) {
            continue;
        }


        string request;

        char buffer[8192];

        int bytesReceived;


        // Receive request
        do {

            bytesReceived = recv(
                clientSocket,
                buffer,
                sizeof(buffer),
                0
            );

            if (bytesReceived > 0) {

                request.append(
                    buffer,
                    bytesReceived
                );

            }

        } while (
            bytesReceived == sizeof(buffer)
        );


        // Handle browser preflight request
        if (request.find("OPTIONS") == 0) {

            sendResponse(
                clientSocket,
                "{}"
            );

            closesocket(clientSocket);

            continue;
        }


        // Only accept POST /analyze
        // Handle BEFORE vs AFTER code comparison
if (request.find("POST /compare") == 0) {

    string body = getRequestBody(request);

cout << "COMPARE BODY LENGTH: " << body.length() << endl;

    // Separate before and after code
    const string separator =
        "CODEMEDIC_SPLIT";

    size_t separatorPos =
        body.find(separator);

    if (separatorPos == string::npos) {

        sendResponse(
            clientSocket,
            "{\"error\":\"Invalid comparison data received.\"}"
        );

        closesocket(clientSocket);
        continue;
    }

    string beforeCode =
        body.substr(0, separatorPos);

    string afterCode =
        body.substr(
            separatorPos + separator.length()
        );

    if (beforeCode.empty() || afterCode.empty()) {

        sendResponse(
            clientSocket,
            "{\"error\":\"Both before and after code are required.\"}"
        );

        closesocket(clientSocket);
        continue;
    }

    // Save BEFORE code temporarily
    string beforeFile =
        "backend_compare_before.cpp";

    ofstream beforeStream(beforeFile);

    beforeStream << beforeCode;
    beforeStream.close();

    // Save AFTER code temporarily
    string afterFile =
        "backend_compare_after.cpp";

    ofstream afterStream(afterFile);

    afterStream << afterCode;
    afterStream.close();

    // Analyze BEFORE code
    CodeAnalyzer beforeAnalyzer;

    if (!beforeAnalyzer.loadFile(beforeFile)) {

        sendResponse(
            clientSocket,
            "{\"error\":\"Could not analyze before code.\"}"
        );

        DeleteFileA(beforeFile.c_str());
        DeleteFileA(afterFile.c_str());

        closesocket(clientSocket);
        continue;
    }

    CodeMetrics beforeMetrics =
        beforeAnalyzer.analyze();

    // Analyze AFTER code
    CodeAnalyzer afterAnalyzer;

    if (!afterAnalyzer.loadFile(afterFile)) {

        sendResponse(
            clientSocket,
            "{\"error\":\"Could not analyze after code.\"}"
        );

        DeleteFileA(beforeFile.c_str());
        DeleteFileA(afterFile.c_str());

        closesocket(clientSocket);
        continue;
    }

    CodeMetrics afterMetrics =
        afterAnalyzer.analyze();

    // Compare both analyses
    ComparisonResult comparison =
        comparator.compare(
            beforeMetrics,
            afterMetrics
        );

    // Convert comparison results to JSON
    string comparisonJSON =
        createComparisonJSON(comparison);

    sendResponse(
        clientSocket,
        comparisonJSON
    );

    // Remove temporary files
    DeleteFileA(beforeFile.c_str());
    DeleteFileA(afterFile.c_str());

    closesocket(clientSocket);
    continue;
}
        if (
            request.find("POST /analyze") == 0
        ) {

            string code = getRequestBody(request);


            if (code.empty()) {

                sendResponse(
                    clientSocket,
                    "{\"error\":\"No C++ source code received.\"}"
                );

                closesocket(clientSocket);

                continue;
            }


            // Save uploaded source code temporarily
            string tempFile = "backend_temp_code.cpp";

            ofstream file(tempFile);

            file << code;

            file.close();


            // Analyze using our existing analyzer
            CodeAnalyzer analyzer;


            if (!analyzer.loadFile(tempFile)) {

                sendResponse(
                    clientSocket,
                    "{\"error\":\"Could not load C++ source file.\"}"
                );

                closesocket(clientSocket);

                continue;
            }


            CodeMetrics metrics =
                analyzer.analyze();
             ComparisonResult comparison =
    comparator.compare(metrics, metrics);


            // Convert results to JSON
            string json =
                createJSON(metrics);


            sendResponse(
                clientSocket,
                json
            );


            // Remove temporary file
            DeleteFileA(
                tempFile.c_str()
            );

        }
        else {

            sendResponse(
                clientSocket,
                "{\"error\":\"Invalid endpoint.\"}"
            );

        }


        closesocket(clientSocket);
    }


    closesocket(serverSocket);

    WSACleanup();

    return 0;
}