#pragma once

// 문자열에서 특정 문자열 찾기
void printSubstringAfterMatch(const char* input, const char* match) {
    const char* findGNRMC = strstr(input, match);

    // 찾은 문자열 뒤의 부분 출력
    if (findGNRMC != NULL) {
        printf("Found match: %s\n", findGNRMC + strlen(match));
    }
    else {
        printf("Match not found\n");
    }
}