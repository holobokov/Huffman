/**
* @file   Huffman.cpp
 * @Author Oleksandr Holobokov (oh318935@student.polsl.pl)
 * @date   29.01.2025
 */

#include <iostream>
#include <fstream>
#include <map>
#include <bitset>
#include <vector>
#include <queue>
#include "Huffman.h"


//! Struktura reprezentująca węzeł drzewa Huffmana
/**
 * @brief Węzeł drzewa Huffmana, przechowuje symbol, częstotliwość i wskaźniki na dzieci.
 */
struct Node {
    char data;
    int freq;
    Node *left;
    Node *right;

    //! Konstruktor domyślny
    Node() : data('\0'), freq(0), left(nullptr), right(nullptr) {
    }

    //! Konstruktor liścia
    /**
     * @param s Symbol (znak).
     * @param i Częstotliwość wystąpień symbolu.
     */
    Node(char s, int i): data(s), freq(i), left(nullptr), right(nullptr) {
    };

    //! Konstruktor węzła wewnętrznego
    /**
     * @param i Suma częstotliwości dzieci.
     * @param l Wskaźnik na lewe dziecko.
     * @param r Wskaźnik na prawe dziecko.
     */
    Node(int i, Node *l = nullptr, Node *r = nullptr): data('\0'), freq(i), left(l), right(r) {
    };
};

//! Struktura porównująca węzły na podstawie częstotliwości
struct Compare {
    /**
     * @brief Operator porównania.
     * @param a Wskaźnik na pierwszy węzeł.
     * @param b Wskaźnik na drugi węzeł.
     * @return true, jeśli częstotliwość `a` jest większa niż `b`.
     */
    bool operator()(const Node *a, const Node *b) {
        return a->freq > b->freq;
    }
};

/**
 * @brief Wyświetla instrukcję obsługi programu.
 */
void help() {
    std::cout << "Usage: -i <input file> -o <output file> -t <k/d> -s <dictionary file>\n";
    std::cout << "Modes:\n";
    std::cout << "  k - compression (default)\n";
    std::cout << "  d - decompression\n";
}

bool getCommands(int argc, char *argv[], std::string &inputFile, std::string &outputFile, std::string &dictionaryFile,
                 char &mode) {
    bool inputFileFlag = false, outputFileFlag = false, dictionaryFileFlag = false, modeFlag = false, iFlag = false,
            oFlag = false, tFlag = false, sFlag = false;
    mode = 'k';

    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-i" && i + 1 < argc) {
            if (!inputFileFlag) {
                inputFile = argv[++i];
                inputFileFlag = true;
                iFlag = true;
            } else {
                help();
                return false;
            }
        }
        if (std::string(argv[i]) == "-o" && i + 1 < argc) {
            if (!outputFileFlag) {
                outputFile = argv[++i];
                outputFileFlag = true;
                oFlag = true;
            } else {
                help();
                return false;
            }
        }
        if (std::string(argv[i]) == "-t" && i + 1 < argc) {
            if (!modeFlag) {
                mode = argv[++i][0];
                if (mode == 'k' || 'd') {
                    modeFlag = true;
                    tFlag = true;
                } else {
                    help();
                    return false;
                }
            } else {
                help();
                return false;
            }
        }
        if (std::string(argv[i]) == "-s" && i + 1 < argc) {
            if (!dictionaryFileFlag) {
                dictionaryFile = argv[++i];
                dictionaryFileFlag = true;
                sFlag = true;
            } else {
                help();
                return false;
            }
        }
    }

    //Sprawdzam czy jakakolwiek flaga została podana
    if (!iFlag || !oFlag || !tFlag || !sFlag) {
        help();
        return false;
    }

    return true;
}

//                                 COMPRESSION
//==================================================================================

/**
 * @brief Buduje drzewo Huffmana na podstawie częstotliwości symboli.
 * @param buffer Bufor zawierający znaki z pliku wejściowego.
 * @return Wskaźnik na korzeń drzewa Huffmana.
 */
Node *buildHuffmanTree(const std::vector<char> &buffer) {
    std::priority_queue<Node *, std::vector<Node *>, Compare> pq;

    //tabela częstotliwości
    std::map<char, int> freq;
    for (char c: buffer) {
        freq[c]++;
    }

    //tworzenie węzłów
    //priority queque, tworzenie kolejki
    for (const auto &[data, frequency]: freq) {
        pq.push(new Node(data, frequency));
    }

    //budowanie drzewa
    while (pq.size() > 1) {
        Node *left = pq.top();
        pq.pop();
        Node *right = pq.top();
        pq.pop();

        Node *parent = new Node(left->freq + right->freq, left, right);
        pq.push(parent);
    }


    return pq.top();
}

/**
 * @brief Przypisuje kody Huffmana liściom drzewa.
 * @param root Korzeń drzewa Huffmana.
 * @param code Aktualny kod binarny.
 * @param huffmanCodes Mapa przechowująca przypisane kody.
 */
void assignCodes(Node *root, std::string code, std::map<char, std::string> &huffmanCodes) {
    if (!root) {
        return;
    }

    // jesli węzel jest lisciem to przypisuje kod
    if (!root->left && !root->right) {
        huffmanCodes[root->data] = code;
    }

    // rekurencyjnie przechodzi po drzewie
    assignCodes(root->left, code + "0", huffmanCodes);
    assignCodes(root->right, code + "1", huffmanCodes);
}


/**
 * @brief Usuwa drzewo Huffmana z pamięci.
 * @param root Korzeń drzewa Huffmana.
 */
void deleteTree(Node *root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

/**
 * @brief Koduje tekst na podstawie kodów Huffmana.
 * @param buffer Bufor zawierający znaki z pliku wejściowego.
 * @param huffmanCodes Mapa przechowująca przypisane kody.
 * @return Zakodowany tekst w formacie binarnym.
 */
std::string encodeText(std::vector<char> &buffer, std::map<char, std::string> &huffmanCodes) {
    std::string encodedText = "";

    for (char c: buffer) {
        if (huffmanCodes.find(c) != huffmanCodes.end()) {
            encodedText += huffmanCodes[c];
        } else {
            std::cerr << "Error: Character '" << c << "' does not have Huffman code.\n";
        }
    }

    //ostatnio dodane
    while (encodedText.length() % 8 != 0) {
        encodedText += "0";
    }

    return encodedText;
}

/**
 * @brief Zapisuje skompresowany tekst do pliku w formie binarnej.
 *
 * Funkcja przetwarza skompresowany tekst (ciąg znaków '0' i '1') na bajty,
 * a następnie zapisuje te bajty w pliku binarnym. Każde 8 bitów (1 bajt)
 * z ciągu wejściowego jest przekształcane na wartość typu `unsigned char`.
 *
 * @param compressedText Skompresowany tekst w postaci ciągu '0' i '1'.
 * @param outputFile Nazwa pliku wyjściowego, w którym dane mają zostać zapisane.
 *
 * @return true, jeśli zapis zakończył się sukcesem, false w przypadku błędu (np. brak dostępu do pliku).
 */
bool writeCompressedText(std::string compressedText, std::string &outputFile) {
    std::ofstream file(outputFile, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error opening file " << outputFile << std::endl;
        return false;
    }

    //vector bajtow
    std::vector<unsigned char> bytes;

    //przetwarzamy tekst na bajty
    for (int i = 0; i < compressedText.size(); i += 8) {
        //tworzymy bajt z 8 bitow (8 znakow z compressedText)
        std::bitset<8> byte(compressedText.substr(i, 8));
        //przeksztalcamy liczbe z unsigned long do unsigned char
        bytes.push_back(static_cast<unsigned char>(byte.to_ulong()));
    }

    //zapisujemy bajty do pliku wejsciowego binarnie
    file.write(reinterpret_cast<const char *>(bytes.data()), bytes.size());

    file.close();
    return true;
}

/**
 * @brief Zapisuje słownik kodowania Huffmana do pliku.
 *
 * Funkcja zapisuje zawartość mapy `huffmanCodes` do pliku tekstowego, gdzie każdy klucz (symbol)
 * jest zapisany wraz z odpowiadającym mu kodem Huffmana w formacie:
 * [symbol][kod][\n]
 *
 * W przypadku symboli specjalnych, takich jak spacja lub znak nowej linii, zapisywane są odpowiednie
 * znaki w pliku (np. spacja pozostaje spacją, a nowa linia jest zapisana w osobnym wierszu).
 *
 * @param huffmanCodes Mapa przechowująca symbole i ich odpowiadające kody Huffmana.
 * @param dictionaryFile Ścieżka do pliku, w którym słownik ma zostać zapisany.
 */
void writeDictionary(std::map<char, std::string> &huffmanCodes, std::string &dictionaryFile) {
    std::ofstream file(dictionaryFile);

    if (!file.is_open()) {
        std::cerr << "Error opening file " << dictionaryFile << std::endl;
        return;
    }

    for (const auto &[key, code]: huffmanCodes) {
        file << key << "" << code << std::endl;
    }

    file.close();
}

//                              DECOMPRESSION
//==================================================================================

/**
 * @brief Odczytuje słownik kodowania Huffmana z pliku.
 *
 * Funkcja odczytuje zawartość pliku tekstowego `dictionaryFile`, w którym każda linia
 * zawiera symbol (lub specjalny znak, jak spacja czy nowa linia) oraz odpowiadający mu kod Huffmana.
 * Mapuje symbole do ich kodów w przekazanej mapie `huffmanCodes`.
 *
 * Format pliku:
 * - [symbol][kod][\n]
 * - Pusta linia reprezentuje znak nowej linii (`\n`), a kolejna linia zawiera odpowiadający mu kod.
 *
 * @param huffmanCodes Mapa, która będzie zawierać odczytane symbole i ich kody Huffmana.
 * @param dictionaryFile Ścieżka do pliku zawierającego słownik kodowania Huffmana.
 */
void readFromDictionary(std::map<char, std::string> &huffmanCodes, std::string &dictionaryFile) {
    std::ifstream file(dictionaryFile);

    if (!file.is_open()) {
        std::cerr << "Error opening file " << dictionaryFile << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            // pusta linia oznacza, ze nastepna linia jest kodem znaku tej linii
            if (std::getline(file, line)) {
                huffmanCodes['\n'] = line;
            } else {
                std::cerr << "Error: Unexpected end of file after empty line." << std::endl;
                break;
            }
        } else {
            // zwykle mapowanie znaku na kod
            char symbol = line[0];
            std::string code = line.substr(1);
            huffmanCodes[symbol] = code;
        }
    }

    file.close();
}

/**
 * @brief Dekoduje ciąg bitów na oryginalny tekst na podstawie słownika Huffmana.
 *
 * Funkcja wykorzystuje mapę `huffmanCodes`, aby dekodować ciąg `bits` (ciąg znaków '0' i '1')
 * na oryginalny tekst, który jest zapisywany w `recreatedText`. Dla każdego fragmentu bitów,
 * funkcja dopasowuje kod do odpowiedniego symbolu w słowniku Huffmana.
 *
 * @param huffmanCodes Mapa zawierająca symbole i odpowiadające im kody Huffmana.
 * @param bits Skompresowany ciąg bitów w postaci ciągu znaków '0' i '1'.
 * @param recreatedText Wyjściowy odtworzony tekst, wypełniany przez funkcję.
 */
void recreateText(std::map<char, std::string> &huffmanCodes, std::string &bits, std::string &recreatedText) {
    std::string bit = "";
    for (const char c: bits) {
        bit += c;
        for (const auto &[key, value]: huffmanCodes) {
            if (value == bit) {
                recreatedText += key;
                bit = "";
                break;
            }
        }
    }
}

/**
 * @brief Zapisuje zdekompresowany tekst do pliku wyjściowego.
 *
 * Funkcja zapisuje ciąg znaków zdekompresowanego tekstu do pliku wyjściowego.
 * Jeśli plik nie może zostać otwarty, wyświetlany jest komunikat o błędzie,
 * a funkcja zwraca wartość `false`.
 *
 * @param compressedText Zdekompresowany tekst, który ma zostać zapisany do pliku.
 * @param outputFile Nazwa pliku wyjściowego, w którym tekst zostanie zapisany.
 * @return true, jeśli zapis zakończył się sukcesem; false w przypadku błędu.
 */
bool writeDecompressedText(std::string compressedText, std::string &outputFile) {
    std::ofstream file(outputFile);

    if (!file.is_open()) {
        std::cerr << "Error opening file " << outputFile << std::endl;
        return false;
    }

    file << compressedText;

    file.close();
    return true;
}


void compressFile(std::string &inputFile, std::string &outputFile, std::string &dictionaryFile) {
    //odczyt pliku
    std::ifstream file(inputFile, std::ios::binary);
    //tekst w postaci charow
    std::vector<char> buffer;
    //mapa z kodami huffmana
    std::map<char, std::string> huffmanCodes;

    if (!file.is_open()) {
        std::cerr << "Error opening file " << inputFile << std::endl;
        return;
    }
    //definiujemy rozmiar plika
    file.seekg(0, std::ifstream::end);
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ifstream::beg);

    //odczytujemy plik do vectora charow buffer
    buffer.resize(fileSize);
    if (!file.read(buffer.data(), fileSize)) {
        std::cerr << "Input file cannot be read" << std::endl;
        return;
    }

    //budujemy drzewo huffmana i przydzielamy kody do liter
    Node *root = buildHuffmanTree(buffer);
    assignCodes(root, "", huffmanCodes);

    std::string compressedText = encodeText(buffer, huffmanCodes);

    //zapis skompresowanego tekstu do pliku
    writeCompressedText(compressedText, outputFile)
        ? std::cout << "Succesfully writed compressed text to " << outputFile << std::endl
        : std::cerr << "Failed to write compressed text to " << outputFile << std::endl;

    //tworzenie slownika
    writeDictionary(huffmanCodes, dictionaryFile);

    deleteTree(root);
    file.close();
}


void decompressFile(std::string &inputFile, std::string &outputFile, std::string &dictionaryFile) {
    std::ifstream file(inputFile, std::ios::binary);
    //kody huffmana
    std::map<char, std::string> huffmanCodes;
    //vector bajtow zczytanych z pliku inputFile
    std::vector<unsigned char> buffer;
    //string bitow dekodowanych z bajtow
    std::string bits;

    //zczytujemy kody huffmana ze slownika
    readFromDictionary(huffmanCodes, dictionaryFile);

    //definiujemy rozmiar plika
    file.seekg(0, std::ifstream::end);
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ifstream::beg);

    //odczytujemy plik do vectora unsigned charow buffer
    buffer.resize(fileSize);
    if (!file.read(reinterpret_cast<char *>(buffer.data()), fileSize)) {
        std::cerr << "Input file cannot be read" << std::endl;
        return;
    }

    //dekodujemy bajty na bity i zapisujemy do std::string bits
    for (unsigned char byte: buffer) {
        bits += std::bitset<8>(byte).to_string();
    }

    std::string recreatedText = "";
    recreateText(huffmanCodes, bits, recreatedText);

    writeDecompressedText(recreatedText, outputFile)
        ? std::cout << "Succesfully writed decompressed text to " << outputFile << std::endl
        : std::cerr << "Failed to write decompressed text to " << outputFile << std::endl;

    file.close();
}
