#include <stdio.h>
#include <stdint.h>

int main() {
	//int32_t lati = 900000002;
	//printf("lati(int32_t): %d", lati);
	float testLati = 3538.843506;
	int calcLatiPart1 = ((int)testLati / 100);
	int calcLatiPart2 = ((int)testLati % 100);
	float calcLatiPart3 = (testLati - (int)testLati);
	float calcLatiPart4 = ((float)calcLatiPart2 + calcLatiPart3);
	float calcLati = 0;

	printf("testLati: %f\n", testLati);
	printf("calcLatiPart1: %d\n", calcLatiPart1);
	printf("calcLatiPart2: %d\n", calcLatiPart2);
	printf("calcLatiPart3: %f\n", calcLatiPart3);
	printf("calcLatiPart4: %f\n", calcLatiPart4);

	return 0;
}