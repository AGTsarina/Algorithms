#include <set>
#include <vector>
using namespace std;

template<typename T>
vector<T> remove_duplicates(const vector<T>& input) {
    set<T> seen;
    vector<T> result;
    
    for (const auto& element : input) {
        if (seen.find(element) == seen.end()) {
            seen.insert(element);
            result.push_back(element);
        }
    }
    
    return result;
}

template<typename T>
void remove_duplicates_inplace(std::vector<T>& input) {
    std::set<T> seen;
    auto new_end = std::remove_if(input.begin(), input.end(), [&seen](const T& value) {
        return !seen.insert(value).second;
    });
    input.erase(new_end, input.end());
}

