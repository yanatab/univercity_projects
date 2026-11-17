#include "radixSort.hpp" //строки сортировать

void RadixSort::radixSort(std::vector<int>& arr){
    if (arr.empty()){
        throw Error(Error::EmptyValueError);
    }
    int max = arr[0];
    for (size_t i = 1; i < arr.size(); i++){
        if (arr[i] > max){
            max = arr[i];
        }
    }
    for(size_t exp = 1; max / exp > 0; exp *= 10){
        std::vector<int> output(arr.size());
        std::vector<int> count(10, 0);

        for (size_t i = 0; i < arr.size(); i++){
            count[(arr[i] / exp) % 10]++;
        }

        for (int i = 1; i < 10; i++){
            count[i] += count[i - 1];
        }

        for (int i = arr.size() - 1; i >= 0; i--){
            output[count[(arr[i] / exp) % 10] - 1] = arr[i];
            count[(arr[i] / exp) % 10]--;
        }

        for (size_t i = 0; i < arr.size(); i++){
            arr[i] = output[i];
        }
    }
}

void RadixSort::sortFromFile(const std::string& inputFilename, const std::string& outputFilename){
    ReadStream stream;
    stream.open(inputFilename);

    std::vector<int> numbers;
    std::string word;

    while (!stream.isEndOfFile()){
        word = stream.readWord();
        if (!word.empty()){
            numbers.push_back(std::stoi(word));
        }
    }
    stream.close();
    if (numbers.empty()){
        std::cout << "No value to sort \n";
        return;
    }
    radixSort(numbers);

    WriteStream outputStream;
    outputStream.open(outputFilename);
    
    for (size_t i = 0; i < numbers.size(); i++){
        outputStream.writeWord(std::to_string(numbers[i]));
        if (i < numbers.size() - 1){
            outputStream.writeChar(' ');
        }
    }
    outputStream.close();
}

void RadixSort::sortFromConsole(const std::string& outputFilename){
    std::vector<int> numbers;
    int num;
    std::cout << "Enter the values: \n";
    while (std::cin >> num) {
        numbers.push_back(num);

        if (std::cin.peek() == '\n'){
            break;
        }
    }
    radixSort(numbers);

    WriteStream outputStream;
    outputStream.open(outputFilename);
    
    for (size_t i = 0; i < numbers.size(); i++){
        outputStream.writeWord(std::to_string(numbers[i]));
        if (i < numbers.size() - 1){
            outputStream.writeChar(' ');
        }
    }
    outputStream.close();
}

void RadixSort::sortRandomFile(const std::string& filename, size_t count, int minVal, int maxVal){
    WriteStream stream;
    srand(time(NULL));
    stream.open(filename);

    for (size_t i = 0; i < count; i++){
        int randNum = rand() % (maxVal - minVal + 1) + minVal;
        stream.writeWord(std::to_string(randNum));
        if (i < count - 1){
            stream.writeChar(' ');
        }
    }
    stream.close();
    
    std::string outputFilename = filename + "_sorted.txt";
    sortFromFile(filename, outputFilename);
    std::cout << "Numbers is generate and sort \n";
}








void RadixSort::radixSortString(std::vector<std::string>& words){
    if (words.empty()){
        return;
    }
    size_t maxLen = 0;
    for (size_t i = 0; i < words.size(); i++){
        if (words[i].length() > maxLen){
            maxLen = words[i].length();
        }
    }

    if (maxLen == 0) return;

    for (int pos = maxLen - 1; pos >= 0; pos--){
        std::vector<std::string> output(words.size());
        std::vector<int> count(256, 0);

        for(size_t i = 0; i < words.size(); i++){
            std::string word = words[i];
            unsigned char c = 0;
            if (pos < word.length()){
                c = (unsigned char)word[pos];
            }
            count[c]++;
        }

        for (size_t i = 1; i < 256; i++){
            count[i] += count[i - 1];
        }

        for (int i = words.size() - 1; i >= 0; i--){
            std::string word = words[i];
            unsigned char c = 0;
            if (pos < word.length()){
                c = (unsigned char)word[pos];
            }
            output[count[c] - 1] = word;
            count[c]--;
        }
        words = output;
    }
}


void RadixSort::sortFromFileString(const std::string& inputFilename, const std::string& outputFilename){
    ReadStream stream;
    stream.open(inputFilename);

    std::vector<std::string> words;
    std::string word;

    while (!stream.isEndOfFile()){
        word = stream.readWord();
        if (!word.empty()){
            words.push_back(word);
        }
    }
    stream.close();
    if (words.empty()){
        std::cout << "No value to sort \n";
        return;
    }
    radixSortString(words);

    WriteStream outputStream;
    outputStream.open(outputFilename);
    
    for (size_t i = 0; i < words.size(); i++){
        outputStream.writeWord(words[i]);
        if (i < words.size() - 1){
            outputStream.writeChar(' ');
        }
    }
    outputStream.close();
}

void RadixSort::sortFromConsoleString(const std::string& outputFilename){
    std::vector<std::string> words;
    std::string word;
    std::cout << "Enter the values: \n";
    while (std::cin >> word) {
        words.push_back(word);

        if (std::cin.peek() == '\n'){
            break;
        }
    }
    radixSortString(words);

    WriteStream outputStream;
    outputStream.open(outputFilename);
    
    for (size_t i = 0; i < words.size(); i++){
        outputStream.writeWord(words[i]);
        if (i < words.size() - 1){
            outputStream.writeChar(' ');
        }
    }
    outputStream.close();
}

void RadixSort::sortRandomFileString(const std::string& filename, size_t count, int minVal, int maxVal){
    WriteStream stream;
    srand(time(NULL));
    stream.open(filename);

    for (size_t i = 0; i < count; i++){
        int randNum = rand() % (maxVal - minVal + 1) + minVal;
        std::string word;

        for (int j = 0; j < randNum; j++){
            word += 'a' + (rand() % 26);
        }
        stream.writeWord(word);
        if (i < count - 1){
            stream.writeChar(' ');
        }
    }
    stream.close();
    
    std::string outputFilename = filename + "_sorted.txt";
    sortFromFileString(filename, outputFilename);
    std::cout << "Words is generate and sort \n";
}
