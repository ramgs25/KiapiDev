#pragma once
//#pragma warning(disable:4996)

// 문자열에서 특정 문자열 찾기
void findGNRMC(const char* input, const char* match) {
    const char* ptr = strstr(input, match);
    const char* onlyGNRMC = 
    //char* divideID = strchr((char*)ptr, '$');
    
    // 찾은 문자열 뒤의 부분 출력
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