/**
* @file   main.cpp
 * @Author Oleksandr Holobokov (oh318935@student.polsl.pl)
 * @date   29.01.2025
 */

#include <iostream>
#include "Huffman.h"

/**
 * @brief Główna funkcja obsługująca kompresję i dekompresję plików za pomocą kodowania Huffmana.
 *
 * @param argc Liczba argumentów wiersza poleceń.
 * @param argv Tablica argumentów wiersza poleceń.
 * @return int Zwraca 0 w przypadku pomyślnego wykonania, 1 w przypadku błędnych argumentów wiersza poleceń.
 */

int main(int argc, char *argv[])
{
    std::string inputFile, outputFile, dictionaryFile;
    char mode = 'k'; // Domyślnie ustawiona kompresja.

    /**
     * @brief Parsuje argumenty wiersza poleceń.
     * @details Funkcja ustawia wartości zmiennych inputFile, outputFile, dictionaryFile oraz mode
     * na podstawie dostarczonych argumentów wiersza poleceń.
     *
     * @param argc Liczba argumentów wiersza poleceń.
     * @param argv Tablica argumentów wiersza poleceń.
     * @param inputFile Nazwa pliku wejściowego do przetworzenia.
     * @param outputFile Nazwa pliku wyjściowego, w którym zostaną zapisane wyniki.
     * @param dictionaryFile Nazwa pliku słownika używanego w kodowaniu Huffmana.
     * @param mode Znak określający tryb działania ('k' dla kompresji, 'd' dla dekompresji).
     * @return bool Zwraca true, jeśli argumenty wiersza poleceń są poprawne, w przeciwnym razie false.
     */
    if (!getCommands(argc, argv, inputFile, outputFile, dictionaryFile, mode)) {
        return 1;
    }

    if (mode == 'k') {
        /**
         * @brief Kompresuje plik wejściowy za pomocą kodowania Huffmana.
         *
         * @param inputFile Nazwa pliku wejściowego do skompresowania.
         * @param outputFile Nazwa pliku wyjściowego, w którym zostaną zapisane dane skompresowane.
         * @param dictionaryFile Nazwa pliku słownika, w którym zostaną zapisane kody Huffmana.
         */
        compressFile(inputFile, outputFile, dictionaryFile);
    } else if (mode == 'd') {
        /**
         * @brief Dekompresuje plik wejściowy za pomocą kodowania Huffmana.
         *
         * @param inputFile Nazwa pliku wejściowego do zdekompresowania.
         * @param outputFile Nazwa pliku wyjściowego, w którym zostaną zapisane dane zdekompresowane.
         * @param dictionaryFile Nazwa pliku słownika zawierającego kody Huffmana.
         */
        decompressFile(inputFile, outputFile, dictionaryFile);
    }

    return 0;
}
