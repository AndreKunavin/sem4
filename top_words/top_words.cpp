#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>

struct Statistics
{
    int count;
    std::string word;

};

bool cmp(Statistics& lhs, Statistics& rhs) {
    return lhs.count > rhs.count;
}
 
void prepare(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    std::size_t len = str.size();
    if (std::ispunct(str[len - 1])) {
        str.pop_back();
    }
}

int main() {
    std::map<std::string, std::size_t> wfr;

    std::ifstream fs("Fellowship of the Ring.txt");
    if (fs.is_open()) {
        while (1){
            std::string word;
            fs >> word;
            if(!fs) {
                break;
            }
            prepare(word);

            auto search = wfr.find(word);
            if (search == wfr.end()) {
                wfr[word] = 1;
            } else {
                ++wfr[word];
            }
        }
    } else {
        std::cout << "File is not opened" << std::endl;
    }
    fs.close();

    std::vector<Statistics> top;
    
    for (auto it = wfr.begin(); it != wfr.end(); it++) {
        Statistics pair;
        pair.word = it->first;
        pair.count = it->second;
        top.push_back(pair);
    }
    
    std::sort(top.begin(), top.end(), cmp);
    for (int i = 0; i < 100; i++) {
        std::cout << top[i].word << " - " << top[i].count << std::endl;
    }

}