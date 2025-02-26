#include<iostream>
#include<string>
#include<cstring>
#include<fstream>
#include<sstream>
#include<vector>

using namespace std;

struct SentenceInfo {
    int lineNumber;
    vector<string> words;
};
vector<SentenceInfo> findSentencesWithWord(const string& filename, const string& word);

int main(int argc, char* argv[]) {
    //Jos ei ole argumentteja, niin käytetään perus inputteja
    if (argc <= 1) {
        string searchInput, contentInput;
        cout << "Give a string from which to search for: ";
        getline(cin, contentInput);
        cout << "Give search string: ";
        getline(cin, searchInput);

        const char* contentInputPtr = contentInput.c_str();
        const char* searchInputPtr = searchInput.c_str();
        //Napataan pituudet inputtien pointtereista
        size_t searchInputLength = strlen(searchInputPtr);
        size_t contentInputLength = strlen(contentInputPtr);
        //Luodaan pointerit inputtien kirjaimille taulukkona
        char* ptr1 = const_cast<char*>(searchInput.data());
        char* ptr2 = const_cast<char*>(contentInput.data());

        //Tehdään molemmmista inputeista pienet kirjaimet
        for (int i = 0; i < searchInputLength; i++) {
            *(ptr1 + i) = tolower(*(ptr1 + i));
        }
        for (int i = 0; i < contentInputLength; i++) {
            *(ptr2 + i) = tolower(*(ptr2 + i));
        }
        //Käydään läpi contentInput kirjaimet ja verrataan sitä searchInputin kirjaimiin
        //Jos löytyy samanlainen kirjain, niin verrataan seuraavia kirjaimia
        //Jos kaikki kirjaimet ovat samat, niin tulostetaan löydetty sana ja sen indeksi
        //Jos ei löydy, niin tulostetaan "NOT found"
            int matchingCharacters = 0;
            for (int i = 0; i < contentInputLength;) {
                if (*(contentInputPtr + i) == *searchInputPtr) {
                    for (int j = 0; j < searchInputLength; j++) {
                        if (*(contentInputPtr + i) == *(searchInputPtr + j)) {
                            matchingCharacters++;
                            i++;
                        }
                        else {
                            i - matchingCharacters;
                            matchingCharacters = 0;
                            break;
                        }
                    }
                }
                //Jos kaikki kirjaimet ovat samat, niin tulostetaan löydetty sana ja sen indeksi ja lopetetaan ohjelma
                if (matchingCharacters == searchInputLength){
                    cout << '"' << searchInput << '"' << " found in " << '"' << contentInput << '"' << " at letter index: "
                     << i - matchingCharacters << endl;
                    return 0;
                }
                i++;
            }
            cout << '"' << searchInput << '"' << " NOT found in " << '"' << contentInput << '"' << '.' << endl;
            return 0;
        }
    
    //Jos on argumentteja, niin käytetään niitä
    if(argc > 1) {
        string searchInput, filePath, commandInput;
        bool isCommand = false;
        bool lineNumbering = false;
        bool englishOption = false;
        bool lineCount = false;
        if (argc < 3) {
            cerr << "Error: Wrong number of arguments" << endl;
            return 1;
        }
        if(argc > 4) {
            cerr << "Error: Wrong number of arguments" << endl;
            return 1;
        }
        //Jos on 3 argumenttia, niin käytetään niitä
        if (argc == 3) {
            searchInput = argv[1];
            filePath = argv[2];
        }
        //Jos on 4 argumenttia, niin käytetään niitä
        if (argc == 4) {
            commandInput = argv[1];
            searchInput = argv[2];
            filePath = argv[3];
            isCommand = true;
        }
        if(isCommand) {
            if(commandInput == "-olo") {
                lineNumbering = true;
                lineCount = true;
                englishOption = true;
            }
            else if(commandInput == "-ol") {
                lineNumbering = true;
                englishOption = true;
            }
            else if(commandInput == "-o") {
                englishOption = true;
            }
            else if(commandInput == "-oo"){
                lineCount = true;
                englishOption = true;
            }
            else {
                cerr << "Error: Unknown command" << endl;
                return 1;
            }
        }

        string contentInput;
        
        vector<SentenceInfo> sentences = findSentencesWithWord(filePath, searchInput);
        //Jos ei löydy sanaa, niin tulostetaan "NOT found"
        if (sentences.empty()) {
            cout << '"' << searchInput << '"' << " NOT found in " << '"' << filePath << '"' << '.' << endl;
            return 0;
        }
        //Jos löytyy sana, niin tulostetaan löydetyt lauseet

        cout << '"' << searchInput << '"' << " found in " << '"' << filePath << '"' << " in the following sentences:" << endl;
        // Print found sentences with line numbers
        if (lineNumbering) {
            for (const auto& sentenceInfo : sentences) {
                cout << "Line " << sentenceInfo.lineNumber << ": ";
                for (const string& word : sentenceInfo.words) {
                    cout << word << " ";
                }
                cout << endl;
            }
        }
        // Print found sentences without line numbers
        else {
            for (const auto& sentenceInfo : sentences) {
                for (const string& word : sentenceInfo.words) {
                    cout << word << " ";
                }
                cout << endl;
            }
        }
        // Print the number of found sentences
        if (lineCount) {
            cout << "Number of sentences found: " << sentences.size() << endl;
        }
        return 0;
}
    return 0;
}

vector<SentenceInfo> findSentencesWithWord(const string& filename, const string& word) {
    ifstream file(filename);
    vector<SentenceInfo> sentences;

    if (!file) {
        cerr << "Error: Cannot open file " << filename << endl;
        return sentences;
    }

    string line;
    int lineNumber = 0;
    while (getline(file, line)) { 
        istringstream iss(line);
        vector<string> words;
        string temp;


        bool containsWord = false;
        while (iss >> temp) {  
            words.push_back(temp);
            if (temp == word) {
                containsWord = true;  
            }
        }
        if (containsWord) {
            sentences.push_back({lineNumber, words});  
        }
        lineNumber++;
    }

    return sentences;
}