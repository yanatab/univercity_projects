#ifndef INTERFACE_HPP
#define INTERFACE_HPP
/*для чтения добавить радиксную сортировку (или может сделать отдельное меню для него (заполнить файл рандомными числами и отсортировать, из выбранного файла, заполнить вручную? все вместе))*/
#include <iostream>
#include <string>
#include <limits>
#include <iostream>
#include "error.hpp"
#include "readStream.hpp"
#include "writeStream.hpp"
#include "ReadWriteStream.hpp"
#include "radixSort.hpp"



using namespace std;

void mainMenu(){
    cout << "MAIN MENU \n";
    cout << "1. Read Stream \n";
    cout << "2. Write Stream \n";
    cout << "3. Mixed Stream \n";
    cout << "4. Radix Sort\n";
    cout << "0. Exit \n";
}

void ReadStreamMenu(){
    cout << "READ STREAM MENU\n";
    cout << "1. Open fail \n";
    cout << "2. Close fail \n";
    cout << "3. Read one char \n";
    cout << "4. Read a word \n";
    cout << "5. Read a line \n";
    cout << "6. Check is it end of file? \n";
    cout << "7. Get position \n";
    cout << "8. Get count of reading char \n";
    cout << "9. Seek by position \n";
    cout << "10. Get name of file \n";
    cout << "11. Check is file open \n";
    cout << "0. Return in main menu \n";
}

void WriteStreamMenu(){ 
    cout << "WRITE STREAM MENU \n";
    cout << "1. File open\n";
    cout << "2. File close\n";
    cout << "3. Write a char\n";
    cout << "4. Write a word\n";
    cout << "5. Write a line\n";
    cout << "6. Get a writting count of char\n";
    cout << "7. Get name of file\n";
    cout << "8. ChecK is file open\n";
    cout << "0. Return back in main menu\n";
}

void MixedStreamMenu(){
    cout << "MIXED STREAM MENU\n";
    cout << "1. File open\n";
    cout << "2. File close\n";
    cout << "3. Switch to read\n";
    cout << "4. Switch to write\n";
    cout << " Operation for reading\n";
    cout << "5. Read a char\n";
    cout << "6. Read a word\n";
    cout << "7. Read a line\n";
    cout << "8. Check is it end of file\n";
    cout << "9. Get position fron the file\n";
    cout << "10. Seek by position\n";
    cout << " Operation for writting\n";
    cout << "11. Write a char\n";
    cout << "12. Write a word\n";
    cout << "13. Write a line\n";
    cout << "14. Get a writting count of char\n";
    cout << " Common information\n";
    cout << "15. Get name of file\n";
    cout << "16. Check is it open\n";
    cout << "17. Check is it read or write\n";
    cout << "0. Return back in main menu\n";
}

void SortedMenu(){
    cout << "RADIX SORT MENU\n";
    cout << "1. Sort numbers from file \n";
    cout << "2. Sort number from console input \n";
    cout << "3. Generate random numbers file and sort it \n";
    cout << "4. Sort line from file \n";
    cout << "5. Sort line from console input \n";
    cout << "6. Generate random numbers file and sort it \n";
    cout << "0. Return to main menu\n";
}

void workWithReadStream(){
    ReadStream stream;
    int choice;

    do {
        ReadStreamMenu();
        cin >> choice;
        if (choice != 1 && choice != 2 && choice != 11 && choice != 0) {
            if (!stream.streamOpen()) {
                cout << "Error: File is not opened. Please select option 1 to open file first.\n";
                continue;
            }
        }
        try{
            switch(choice){
                case 1:{
                    cout << "Enter file name: ";
                    string filename;
                    cin >> filename;
                    stream.open(filename);
                    cout << "\nFile " <<  filename << " is open \n";
                    break;
                }
                case 2:{
                    stream.close();
                    cout << "File is close";
                    break;
                }
                case 3:{
                    char c = stream.readChar();
                    cout << "Char is read: " << c << "\n";
                    break;
            }
                case 4:{
                    string word = stream.readWord();
                    cout << "Word is read: " << word << "\n";
                    break;
                }
                case 5:{
                    string line = stream.readLine();
                    cout << "Line is read: " << line <<"\n";
                    break;
                }
                case 6:{
                    if (stream.isEndOfFile()){
                        cout << "It is the last character \n";
                    }
                    else cout << "It is not last character \n";
                    break;
                }
                case 7:{
                    cout << "Position in the file: " << stream.getPosition() << "\n";
                    break;
                }
                case 8:{
                    cout << "Char count: " << stream.getCharCount() << "\n";
                    break;
                }
                case 9:{
                    cout << "Write position for seek: ";
                    size_t position;
                    cin >> position;
                    stream.Seek(position);
                    cout << "Seek to position " << position << "\n";
                    break;
                }
            case 10:{
                    cout << "Name of the file is " << stream.getFileName() << "\n";
                    break;
                }
                case 11:{
                    if (stream.streamOpen()){
                        cout << "File is open \n";
                    }
                    else cout << "File is close \n";
                    break;
                }case 0:{
                    cout << "Return back";
                    break;
                }
                default:
                cout << "Mistake with action\n";
            }
        } catch (Error e) {
            errorDetection(e);
        }
    } while (choice != 0);
};

void workWithWriteStream(){
    WriteStream stream;
    int choice;

    do{
        WriteStreamMenu();
        cin >> choice;
         if (choice != 1 && choice != 2 && choice != 8 && choice != 0) {
            if (!stream.streamOpen()) {
                cout << "Error: File is not opened. Please select option 1 to open file first.\n";
                continue;
            }
        }
        try{
            switch (choice){
                case 1:{
                    string filename;
                    cin >> filename;
                    stream.open(filename);
                    cout << "File " <<  filename << "is open \n";
                    break;
                }
                case 2:{
                    stream.close();
                    cout << "File is close";
                    break;
                }
                case 3:{
                    char c;
                    cin >> c;
                    stream.writeChar(c);
                    cout << "Char " << c << " is write \n";
                    break;
                }
                case 4:{
                    string word;
                    cin >> word;
                    stream.writeWord(word);
                    cout << "Word " << word << " is write\n";
                    break;
                }
                case 5:{
                    string line;
                    cout << "Enter the line: \n";
                    getline(cin, line);
                    stream.writeLine(line);
                    cout << "Line " << line <<" is write\n";
                    break;
                }
                case 6:{
                    cout << "Count of writting char " << stream.getCountWrite() << "\n";
                    break;
                }
                
                case 7:{
                    cout << "Name of the file is " << stream.getFileName() << "\n";
                    break;
                }
                case 8:{
                    if (stream.streamOpen()){
                        cout << "File is open \n";
                    }
                    else cout << "File is close \n";
                    break;
                }case 0:{
                    cout << "Return back";
                    break;
                }
                default:
                cout << "Mistake with action\n";
            }
        } catch (Error e) {
            errorDetection(e);
        }
    
    } while (choice != 0);
}

void workWithMixedStream(){
    ReadWriteStream stream;
    int choice;

    do {
        MixedStreamMenu();
        cin >> choice;

        if (choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 16 && choice != 17 && choice != 0) {
            if (!stream.streamOpen()) {
                cout << "Error: File is not opened. Please select option 1 to open file first.\n";
                continue;
            }
        }
        try{
            switch (choice){
                case 1:{
                    string filename;
                    cin >> filename;
                    stream.open(filename);
                    cout << "File " <<  filename << "is open \n";
                    break;
                }
                case 2:{
                    stream.close();
                    cout << "File is close";
                    break;
                }
                case 3:{
                    stream.switchToRead();
                    cout << "Now you work with Read Stream \n";
                    break;
                }
                case 4:{
                    stream.switchToWrite();
                    cout << "You work with Write Stream \n";
                    break;
                }
                case 5:{
                    char c = stream.readChar();
                    cout << "Char is read: " << c << "\n";
                    break;
                }
                case 6:{
                    string word = stream.readWord();
                    cout << "Word is read: " << word << "\n";
                    break;
                }
                case 7:{
                    string line = stream.readLine();
                    cout << "Line is read: " << line <<"\n";
                    break;
                }
                case 8:{
                    if (stream.isEndOfFile()){
                        cout << "It is the last character \n";
                    }
                    else cout << "It is not last character \n";
                    break;
                }
                case 9:{
                    cout << "Position in the file: " << stream.getPosition() << "\n";
                    break;
                }
                case 10:{
                    cout << "Write position for seek: ";
                    size_t position;
                    cin >> position;
                    stream.Seek(position);
                    cout << "Seek to position " << position << "\n";
                    break;
                }
                
                case 11:{
                    char c;
                    cin >> c;
                    stream.writeChar(c);
                    cout << "Char " << c << " is write \n";
                    break;
                }
                case 12:{
                    string word;
                    cin >> word;
                    stream.writeWord(word);
                    cout << "Word " << word << " is write\n";
                    break;
                }
                case 13:{
                    string line;
                    cin >> line;
                    stream.writeLine(line);
                    cout << "Line " << line <<" is write\n";
                    break;
                }
                case 14:{
                    cout << "Count of writting char " << stream.getCountWrite() << "\n";
                    break;
                }
                case 15:{
                    cout << "Name of the file is " << stream.getFileName() << "\n";
                    break;
                }
                case 16:{
                    if (stream.streamOpen()){
                        cout << "File is open \n";
                    }
                    else cout << "File is close \n";
                    break;
                }
                case 17:{
                    cout << "Which mode is chosen? \n";
                    if (stream.isReadingMode()){
                        cout << "It is read mode\n";
                    }
                    else if (stream.isWriteMode()){
                        cout << "It is write mode\n";
                    }
                    cout << "Nothing, choose the mode (write or read)\n";
                break;
                }
                case 0:{
                    cout << "Return back";
                    break;
                }
                default:
                cout << "Mistake with action\n";
            }
        } catch (Error e) {
            errorDetection(e);
        }

    } while (choice != 0);
}
void workWithRadixSort(){
    int choice;
    RadixSort sorter;

    do{
        SortedMenu();
        cin >> choice;
        try{
            switch (choice){
                case 1: {
                    cout << "Enter input filename: \n";
                    string inputFilename;
                    cin >> inputFilename;

                    cout << "Enter output filename for sorted numbers: ";
                    string outputFilename;
                    cin >> outputFilename;
                    sorter.sortFromFile(inputFilename, outputFilename);
                    break;
                }
                case 2:{
                    cout << "Enter output filename for sorted numbers: ";
                    string outputFilename;
                    cin >> outputFilename;
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    sorter.sortFromConsole(outputFilename);
                    break;
                }
                case 3:{
                    cout << "Enter filename for random numbers: ";
                    string filename;
                    cin >> filename;
                    
                    cout << "Enter count of numbers: ";
                    int count;
                    cin >> count;
                    
                    cout << "Enter min value: ";
                    int minVal;
                    cin >> minVal;
                    
                    cout << "Enter max value: ";
                    int maxVal;
                    cin >> maxVal;
                    
                    sorter.sortRandomFile(filename, count, minVal, maxVal);
                    break;
                }
                case 4: {
                    cout << "Enter input filename: \n";
                    string inputFilename;
                    cin >> inputFilename;

                    cout << "Enter output filename for sorted line: ";
                    string outputFilename;
                    cin >> outputFilename;
                    sorter.sortFromFileString(inputFilename, outputFilename);
                    break;
                }
                case 5:{
                    cout << "Enter output filename for sorted numbers: ";
                    string outputFilename;
                    cin >> outputFilename;
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    sorter.sortFromConsoleString(outputFilename);
                    break;
                }
                case 6:{
                    cout << "Enter filename for random line: ";
                    string filename;
                    cin >> filename;
                    
                    cout << "Enter count of words: ";
                    int count;
                    cin >> count;
                    
                    cout << "Enter min length of word: ";
                    int minVal;
                    cin >> minVal;
                    
                    cout << "Enter max length of word: ";
                    int maxVal;
                    cin >> maxVal;
                    
                    sorter.sortRandomFileString(filename, count, minVal, maxVal);
                    break;
                }
                case 0:{
                    cout << "Return to main menu\n";
                    break;
                }
                default:
                    cout << "Invalid choice\n";
            }

        } catch (Error e) {
            errorDetection(e);
        }
    } while (choice != 0);
}

void runInterface(){
    int choice;
    do{
        mainMenu();
        cin >> choice;
        switch(choice){
            case 1:{
                workWithReadStream();
                break;
            }
            case 2:{
                workWithWriteStream();
                break;
            }
            case 3:{
                workWithMixedStream();
                break;
            }
            case 4:{
                workWithRadixSort();
                break;
            }
            case 0:{
                cout << "Exit\n";
                break;
            }
            default:
            cout << "Mistake with action, choose ofter \n";
        }
    }   while (choice != 0);
}

#endif 