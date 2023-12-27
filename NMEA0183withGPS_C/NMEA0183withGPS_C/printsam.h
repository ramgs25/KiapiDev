#pragma once

void printSubstringAfterMatch(const char* input, const char* match) {
    // 문자열에서 특정 문자열 찾기
    const char* findGPRMC = strstr(input, match);

    if (findGPRMC != NULL) {
        // 찾은 문자열 뒤의 부분 출력
        printf("Found match: %s\n", findGPRMC + strlen(match));
    }
    else {
        printf("Match not found\n");
    }
}