#pragma once

// ���ڿ����� Ư�� ���ڿ� ã��
void printSubstringAfterMatch(const char* input, const char* match) {
    const char* findGNRMC = strstr(input, match);

    // ã�� ���ڿ� ���� �κ� ���
    if (findGNRMC != NULL) {
        printf("Found match: %s\n", findGNRMC + strlen(match));
    }
    else {
        printf("Match not found\n");
    }
}