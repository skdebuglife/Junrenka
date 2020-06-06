// main.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <stdio.h>
#include "CJunrenka.h"
#include <locale.h>

int main()
{
    setlocale(LC_ALL, "");

    CJunrenka junrenka;

    junrenka.StartLyrics();

    return 0;
}
