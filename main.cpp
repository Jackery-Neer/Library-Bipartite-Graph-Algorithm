#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

void trendingBookRecommendations(const unordered_map<string, int>& bookPair, const unordered_set<string>& userSet) {
    vector<pair<int, string>> mostPopular;
    for (const auto& [book, count] : bookPair) {
        if (userSet.find(book) == userSet.end()) {
            mostPopular.push_back({count, book});
        }
    }
    sort(mostPopular.rbegin(), mostPopular.rend());

    cout << "\nThe Most Trending Books:\n";
    int limit = min((int)mostPopular.size(), 3);
    for (int i = 0; i < limit; i++) {
        cout << i + 1 << ". " << mostPopular[i].second << " (Has " << mostPopular[i].first << " borrows\n";
    }
}

vector<string> getRecommendations(const vector<string>& neighborBooks, const unordered_set<string>& userSet) {
    vector<string> recs;
    for (const string& book : neighborBooks) {
        if (userSet.find(book) == userSet.end()) {
            recs.push_back(book);
        }
    }
    return recs;
}

void handleFeedback(const string& nextBestMatch, int nextSimilar, const unordered_map<string, vector<string>>& Graph, const unordered_set<string>& userSet, const unordered_map<string, int>& bookPair) {
    
    cout << "\nDo you like these recommendations? (Yes/No): ";
    string choice;
    cin >> choice;

    if (choice == "Yes" || choice == "yes") {
        cout << "Awesome! Enjoy your new reads.\n";
    } else {
        if (nextBestMatch != "") {
            cout << "\nNo problem! The next user most similar to you is " << nextBestMatch << " with " << nextSimilar << " matches.\n";
            vector<string> nextRecs = getRecommendations(Graph.at(nextBestMatch), userSet);
            
            cout << "Their recommended books:\n";
            for (const string& book : nextRecs) cout << "- " << book << endl;
            cout << "\nIf those don't work, here is what's generally popular:";
            trendingBookRecommendations(bookPair, userSet);
        } else {
            trendingBookRecommendations(bookPair, userSet);
        }
    }
}

int main() {
    unordered_map<string, vector<string>> Graph;
    vector<string> roseBorrow = {"Animal Farm", "1984", "Lord of the Flies", "Harry Potter and the Philosopher's Stone", "The Hobbit", "The Fellowship of the Ring"};
    vector<string> billBorrow = {"Harry Potter and the Philosopher's Stone", "Harry Potter and the Chamber of Secrets", "Harry Potter and the Prisoner of Azkaban"};
    vector<string> judithBorrow = {"The Fellowship of the Ring", "The Two Towers", "The Return of the King", "Crime and Punishment", "Chainsawman Vol. 1"};
    vector<string> marcusBorrow = {"Meditations on First Philosophy", "Ethics", "The Critique of Pure Reason", "Beyond Good and Evil"};
    vector<string> johnBorrow = {"The Bible", "The Quran", "The Fellowship of the Ring", "Chainsawman Vol. 1", "Chainsawman Vol. 2", "Jujutsu Kaisen Vol. 1"};
    vector<string> userBorrow;

    Graph["Rose"] = roseBorrow;
    Graph["Bill"] = billBorrow;
    Graph["Judith"] = judithBorrow;
    Graph["Marcus"] = marcusBorrow;
    Graph["John"] = johnBorrow;

    vector<string> userCatalog = {"Rose", "Bill", "Judith", "Marcus", "John"};

    cout << "Welcome to the Library Recommendation System";

    cout << "What is your name?\n";
    string name;
    cin >> name;
    userCatalog.push_back(name);

    
    cout << "How many books have you borrowed?\n";
    int numBooks;
    cin >> numBooks;
    cin.ignore();

    for (int i = 0; i < numBooks; i++) {
        string book;
        cout << "Book " << i + 1 << ":\n";
        getline(cin, book);
        userBorrow.push_back(book);

    }

    unordered_set<string> userSet(userBorrow.begin(), userBorrow.end());

    string bestMatch = "";
    string nextBestMatch = "";
    int nextSimilar = -1;
    int maxSimilar = -1;
    unordered_map<string, int> bookPair;

    for (const auto& [neighbor, books] : Graph) {
        int currentSimilar = 0;
        bool hasNewRecommendation = false;
    
        for (const string& book : books) {
            bookPair[book]++;
            if (userSet.find(book) != userSet.end()) {
                currentSimilar++;
            } else {
                hasNewRecommendation = true;
            }
        }


        if ((currentSimilar > maxSimilar) && hasNewRecommendation) {
            nextSimilar = maxSimilar;
            maxSimilar = currentSimilar;
            nextBestMatch = bestMatch;
            bestMatch = neighbor;
        } else if (hasNewRecommendation && currentSimilar > nextSimilar) {
            nextSimilar = currentSimilar;
            nextBestMatch = neighbor;
        }
    }
    if (bestMatch != "") {
        cout << "Based on your past recommendations " << bestMatch << "'s list is the most similar.\n";
        cout << bestMatch << " has " << maxSimilar << " similarly borrowed books to you";
    } else {
        cout << "You have no similar books to anybody.\n";
        trendingBookRecommendations(bookPair, userSet);
        handleFeedback(nextBestMatch, nextSimilar, Graph, userSet, bookPair);
        return 0;
    }
    vector<string> recommendations = getRecommendations(Graph[bestMatch], userSet);

    cout << "We recommend these books: \n";
    for (int i = 0; i < recommendations.size(); i++) {
        cout << recommendations.at(i) << endl;
    }
    
    handleFeedback(nextBestMatch, nextSimilar, Graph, userSet, bookPair);

    return 0;
}