#pragma once

void printSubstringAfterMatch(const char* input, const char* match) {
    // ���ڿ����� Ư�� ���ڿ� ã��
    const char* findGPRMC = strstr(input, match);

    if (findGPRMC != NULL) {
        // ã�� ���ڿ� ���� �κ� ���
        printf("Found match: %s\n", findGPRMC + strlen(match));
    }
    else {
        printf("Match not found\n");
    }
}