#pragma once
//#pragma warning(disable:4996)

// ���ڿ����� Ư�� ���ڿ� ã��
void findGNRMC(const char* input, const char* match) {
    const char* ptr = strstr(input, match);
    const char* onlyGNRMC = 
    //char* divideID = strchr((char*)ptr, '$');
    
    // ã�� ���ڿ� ���� �κ� ���
    if (ptr != NULL) {
        //printf("Found match: %s\n", ptr + strlen(match));
        printf("%s\n", onlyGNRMC);
        //printf("divideID result: %s\n\n", divideID);
        //printf("%s\n\n", onlyGNRMC);
    }
    else {
        printf("Match not found\n");
    }
}

/*void cutEssential(char* (input), char match) {
    char* onlyGNRMC = strchr(input, match);

    while(onlyGNRMC != NULL){
        printf("%s\n", onlyGNRMC);
        onlyGNRMC = strchr(onlyGNRMC
}*/