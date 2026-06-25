#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include "MyHeader.c"

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
	loadscreen();
    Print_Main();
    return 0;
}
