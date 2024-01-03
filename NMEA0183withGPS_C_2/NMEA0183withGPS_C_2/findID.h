#pragma once

//main���� findGNRMC() �Լ��� ������ ���� ���ڿ� �Բ� ȣ���� ����
//findGNRMC(buffer, "$GNRMC", '$');

void findGNRMC(const char* input, const char* match, char delimiter) {
	// ���ڿ�input���� match���ڿ��� ã��
	char* result = strstr((char*)input, match);

	if (result != NULL) {
		//ã�� ��ġ���� ���� NMEA0183 ID($XXXXX..)�� ��Ÿ�� �������� �κ� ���ڿ��� ���
		printf("found match: ");
		while (*result != delimiter) {
			printf("%s", result);
		}
		printf("\n");
	}
	else {
		printf("match not found.\n");
	}
}