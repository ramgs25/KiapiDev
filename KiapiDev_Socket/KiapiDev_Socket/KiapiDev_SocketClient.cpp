#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

int main() {
	SOCKET		s;
	WSADATA		wsaData;
	SOCKADDR_IN sin;

	if (WSAStartup(WINSOCK_VERSION, &wsaData) != 0) {
		printf("WSAStartup 실패, 에러코드: %d\n", WSAGetLastError());
		return 0;
	}

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (s == INVALID_SOCKET) {
		printf("소켓 생성 실패, 에러코드: %d\n", WSAGetLastError());
		WSACleanup(); return 0;
	}

	sin.sin_family = AF_INET;
	sin.sin_port = htons(1234);
	sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	if (connect(s, (SOCKADDR*)&sin, sizeof(sin)) != 0) {
		printf("접속 실패, 에러코드: %d\n", WSAGetLastError());
		return 0;
	}

	printf("서버와 연결되었습니다.\n 서버에 전송할 데이터 입력: ");

	char buffer[1024];
	scanf("%s", buffer);

	send(s, buffer, strlen(buffer) + 1, 0);

	printf("데이터 전송 성공!\n");

	recv(s, buffer, 1024, 0);

	printf("[서버에서 보낸 데이터]\n%s\n", buffer);

	if (closesocket(s) != 0) {
		printf("소켓 제거 실패, 에러코드: %d\n", WSAGetLastError());
		WSACleanup(); return 0;
	}
	if (WSACleanup() != 0) {
		printf("WSACleanup 실패, 에러코드: %d\n", WSAGetLastError());
		return 0;
	}

	return 0;
}