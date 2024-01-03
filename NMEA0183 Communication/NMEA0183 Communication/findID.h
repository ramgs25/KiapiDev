#pragma once

//main���� findGNRMC() �Լ��� ������ ���ڰ� findGNRMC(buffer, "$GNRMC", '$')�� ȣ���� ����

void findGNRMC(const char* input, const char* match, char delimiter) {
	//���ڿ�input���� match���ڿ��� ã��
	char* result = strstr((char*)input, match);

	if (*result != NULL) {
		//���ڿ� ���� �� ���� �߰�
		result[strcspn(result, "\n")] = '\0';
		//�� �ּ� ���� ã�� ��ġ���� ���� NMEA0183 ID($XXXXX..)�� ��Ÿ�� �������� �κ� ���ڿ��� ���
		//�� �ڵ� ���� printf("found match: %s\n\n", result);
		return result;
	}
	else {
		printf("match not found.\n");
	}
}