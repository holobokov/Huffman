/**
* @file   Huffman.h
 * @Author Oleksandr Holobokov (oh318935@student.polsl.pl)
 * @date   29.01.2025
 */

#include <iostream>

#ifndef HUFFMAN_H
#define HUFFMAN_H

/**
 * @brief Funkcja przetwarza argumenty wiersza poleceń i weryfikuje ich poprawność.
 *
 * Funkcja analizuje argumenty wiersza poleceń, przypisuje wartości do odpowiednich parametrów,
 * sprawdza ich poprawność i ustawia domyślny tryb działania (jeśli nie podano trybu).
 * W przypadku błędnych danych lub braku wymaganych argumentów, funkcja zwraca `false`.
 *
 * @param argc Liczba argumentów wiersza poleceń.
 * @param argv Tablica argumentów wiersza poleceń.
 * @param inputFile Ścieżka do pliku wejściowego (parametr wyjściowy).
 * @param outputFile Ścieżka do pliku wyjściowego (parametr wyjściowy).
 * @param dictionaryFile Ścieżka do pliku ze słownikiem (parametr wyjściowy).
 * @param mode Tryb działania programu ('k' dla kompresji, 'd' dla dekompresji) (parametr wyjściowy).
 *
 * @return `true` jeśli wszystkie argumenty są poprawne, `false` w przypadku błędu.
 */
bool getCommands(int argc, char* argv[], std::string& inputFile, std::string& outputFile, std::string& dictionaryFile, char& mode);

/**
 * @brief Funkcja kompresuje plik za pomocą kodowania Huffmana.
 *
 * Funkcja odczytuje zawartość pliku wejściowego, buduje drzewo Huffmana na podstawie
 * częstotliwości znaków, przypisuje kody binarne i zapisuje skompresowany tekst
 * oraz słownik kodowania do odpowiednich plików.
 *
 * @param inputFile Ścieżka do pliku wejściowego.
 * @param outputFile Ścieżka do pliku wyjściowego.
 * @param dictionaryFile Ścieżka do pliku ze słownikiem kodowania.
 */
void compressFile(std::string &inputFile, std::string& outputFile, std::string &dictionaryFile);

/**
 * @brief Funkcja dekompresuje plik za pomocą kodowania Huffmana.
 *
 * Funkcja odczytuje plik wejściowy oraz słownik kodowania, a następnie
 * odtwarza oryginalną zawartość pliku na podstawie kodów Huffmana.
 *
 * @param inputFile Ścieżka do pliku wejściowego (skompresowanego).
 * @param outputFile Ścieżka do pliku wyjściowego (zdekompresowanego).
 * @param dictionaryFile Ścieżka do pliku ze słownikiem kodowania.
 */
void decompressFile(std::string &inputFile, std::string& outputFile, std::string &dictionaryFile);

#endif
