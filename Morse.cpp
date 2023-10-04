#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class MorseSource {
protected:
    string morseCode;
    string englishText;

public:
    MorseSource(const string& code = "") : morseCode(code), englishText("") {}

    void translate() {
        // Implement the translation in derived classes
    }

    void displayTranslation() const {
        cout << "Input Text: " << morseCode << endl;
        cout << "Translation: " << englishText << endl;
    }
};

class TextualMorseSource : public MorseSource {
private:
    map<string, char> morseToEnglish;
    map<char, string> englishToMorse;

public:
    TextualMorseSource(const string& code = "") : MorseSource(code) {
        // Initialize Morse to English and English to Morse mappings
        morseToEnglish = {
            {".-", 'A'}, {"-...", 'B'}, {"-.-.", 'C'}, {"-..", 'D'}, {".", 'E'},
            {"..-.", 'F'}, {"--.", 'G'}, {"....", 'H'}, {"..", 'I'}, {".---", 'J'},
            {"-.-", 'K'}, {".-..", 'L'}, {"--", 'M'}, {"-.", 'N'}, {"---", 'O'},
            {".--.", 'P'}, {"--.-", 'Q'}, {".-.", 'R'}, {"...", 'S'}, {"-", 'T'},
            {"..-", 'U'}, {"...-", 'V'}, {".--", 'W'}, {"-..-", 'X'}, {"-.--", 'Y'},
            {"--..", 'Z'}, {"/", ' '} // Space
        };

        // Initialize English to Morse mapping (reverse)
        for (const auto& pair : morseToEnglish) {
            englishToMorse[pair.second] = pair.first;
        }
    }

    // Overload the translate function for Morse to English
    void translate(bool morseToEng) {
        if (morseToEng) {
            morseToEnglishTranslate();
        } else {
            englishToMorseTranslate();
        }
    }

private:
    void morseToEnglishTranslate() {
        string currentSymbol;
        englishText = "";

        for (size_t i = 0; i < morseCode.length(); ++i) {
            if (morseCode[i] == ' ' || morseCode[i] == '/') {
                if (!currentSymbol.empty()) {
                    char englishChar = '?'; // Default for unrecognized Morse symbol
                    auto it = morseToEnglish.find(currentSymbol);
                    if (it != morseToEnglish.end()) {
                        englishChar = it->second;
                    }
                    englishText += englishChar;
                    currentSymbol = "";
                }
                if (morseCode[i] == '/') {
                    englishText += ' '; // Word boundary
                }
            } else {
                currentSymbol += morseCode[i];
            }
        }

        // Process the last Morse symbol
        if (!currentSymbol.empty()) {
            char englishChar = '?'; // Default for unrecognized Morse symbol
            auto it = morseToEnglish.find(currentSymbol);
            if (it != morseToEnglish.end()) {
                englishChar = it->second;
            }
            englishText += englishChar;
        }
    }

    void englishToMorseTranslate() {
        string uppercaseText;
        for (char c : morseCode) {
            if (isalpha(c)) {
                uppercaseText += toupper(c);
            } else if (c == ' ') {
                uppercaseText += '/';
            }
        }

        string morseText;
        for (char c : uppercaseText) {
            if (c == ' ') {
                morseText += ' ';
            } else {
                auto it = englishToMorse.find(c);
                if (it != englishToMorse.end()) {
                    morseText += it->second + ' ';
                }
            }
        }

        englishText = morseText;
    }
};

int main() {
    while (true) {
        cout << "Morse to English Translator Menu:\n";
        cout << "1. Translate Morse to English\n";
        cout << "2. Translate English to Morse\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;
        cin.ignore(); // Consume newline character

        if (choice == 1) {
            string morseInput;
            cout << "Enter Morse code: ";
            getline(cin, morseInput);

            TextualMorseSource textualSource(morseInput);
            textualSource.translate(true); // Translate Morse to English
            textualSource.displayTranslation();
        } else if (choice == 2) {
            string englishInput;
            cout << "Enter English text: ";
            getline(cin, englishInput);

            TextualMorseSource textualSource(englishInput);
            textualSource.translate(false); // Translate English to Morse
            textualSource.displayTranslation();
        } else if (choice == 3) {
            break; // Exit the program
        } else {
            cout << "Invalid choice. Please enter 1, 2, or 3.\n";
        }
    }

    return 0;
}
