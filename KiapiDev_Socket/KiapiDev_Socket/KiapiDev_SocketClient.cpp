#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

int main() {
	SOCKET		s;
	WSADATA		wsaData;
	SOCKADDR_IN sin;

	if (WSAStartup(WINSOCK_VERSION, &wsaData) != 0) {
		printf("WSAStartup ����, �����ڵ�: %d\n", WSAGetLastError());
		return 0;
	}

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (s == INVALID_SOCKET) {
		printf("���� ���� ����, �����ڵ�: %d\n", WSAGetLastError());
		WSACleanup(); return 0;
	}

	sin.sin_family = AF_INET;
	sin.sin_port = htons(1234);
	sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	if (connect(s, (SOCKADDR*)&sin, sizeof(sin)) != 0) {
		printf("���� ����, �����ڵ�: %d\n", WSAGetLastError());
		return 0;
	}

	printf("������ ����Ǿ����ϴ�.\n ������ ������ ������ �Է�: ");

	char buffer[1024];
	scanf("%s", buffer);

	send(s, buffer, strlen(buffer) + 1, 0);

	printf("������ ���� ����!\n");

	recv(s, buffer, 1024, 0);

	printf("[�������� ���� ������]\n%s\n", buffer);

	if (closesocket(s) != 0) {
		printf("���� ���� ����, �����ڵ�: %d\n", WSAGetLastError());
		WSACleanup(); return 0;
	}
	if (WSACleanup() != 0) {
		printf("WSACleanup ����, �����ڵ�: %d\n", WSAGetLastError());
		return 0;
	}

	return 0;
}