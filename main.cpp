#include <iostream>
#include <set>
#include <map>
#include <cmath>
#include <fstream>
#include "csvstream.hpp"
using namespace std;



class Classifier    {
    public:
    Classifier(): numPosts(0), numWords(0)    {}
        void train(string filename, bool debug) {
            csvstream csvIn(filename);
            map <string, string> row;
            if (debug)  {
                cout << "training data:" << endl;
            }
            set <string> words;
            while (csvIn >> row)    {
                allLabels.insert(row["tag"]);
                set <string> tempWords = unique_words(row["content"]);
                for (set<string>::iterator it = tempWords.begin(); 
                    it != tempWords.end(); ++it)   {
                    words.insert(*it);
                } 
                for (auto word : tempWords) {
                    allWords.insert(word);
                }

                if (numPostsWithLabel.find(row["tag"])  
                == numPostsWithLabel.end())   {
                    numPostsWithLabel[row["tag"]] = 1;
                }
                else    {
                    ++numPostsWithLabel[row["tag"]];
                }

                for (auto word: tempWords)  {
                    if (numPostsWithWord.find(word)
                    == numPostsWithWord.end())    {
                        numPostsWithWord[word] = 1;    
                    }
                    else    {
                        ++numPostsWithWord[word];
                    }
                    if (numPostsLabelAndWord.find({row["tag"], word})
                        == numPostsLabelAndWord.end())  {
                        numPostsLabelAndWord[{row["tag"], word}] = 1;
                        allPairs.insert({row["tag"], word});
                    }
                    else    {
                        ++numPostsLabelAndWord[{row["tag"], word}];
                    }
                }
                if (debug)  {
                    cout << "  label = " << row["tag"] << ", content = "
                    << row["content"] << endl;
                }

                ++numPosts;
            }
            numWords = words.size();
            cout << "trained on " << numPosts << " examples\n";
            if (debug)  {
                cout << "vocabulary size = " << numWords << endl;
                debugOut();
            }
        }

        void test(string filename)    {
            csvstream csv(filename);
            map <string, string> row;
            int numCorrect = 0;
            int count = 0;
            cout << "\ntest data:" << endl;
            while (csv >> row)  {
                cout << "  correct = " << row["tag"] << ", ";
                cout << "predicted = " << predicteLabel(row["content"]) << ", ";
                cout << "log-probability score = " 
                << computeProbability(row["content"], predicteLabel(row["content"]));
                cout << endl << "  content = " << row["content"] << endl << endl;

                if (row["tag"] == predicteLabel(row["content"]))  {
                    ++numCorrect;
                }
                ++count;
            }
            cout << "performance: " << numCorrect << " / " 
            << count << " posts predicted correctly";
            cout << endl;
        }
        

    private:
        string predicteLabel(string post)  {
            double max = computeProbability(post, *allLabels.begin());
            string maxLabel = *allLabels.begin();
            for (string label : allLabels)   {
                double probability = computeProbability(post, label);
                if (probability > max)  {
                    max = probability;
                    maxLabel = label;
                }
            }
            return maxLabel;
        }

        double computeProbability(string bagOfWords, string label) {
            double calc = 0;
            set<string> words = unique_words(bagOfWords);
            double total = 0;
            total += log(static_cast<double>(numPostsWithLabel[label])/numPosts);

            for (auto word : words) {
                if (numPostsLabelAndWord.find({label, word})
                == numPostsLabelAndWord.end())  {
                    if (numPostsWithWord.find(word)
                    == numPostsWithWord.end())  {
                        calc = log(1.0/numPosts);
                    }
                    else    {
                        calc = log(static_cast<double>
                        (numPostsWithWord[word])/numPosts);
                    }
                }
                else    {
                    calc = log(static_cast<double>
                    (numPostsLabelAndWord[{label, word}])/numPostsWithLabel[label]);
                }
            total += static_cast<double>(calc);
            }

            return total;
        }


        void debugOut() {
            cout << "classes:\n";
            for (auto label : allLabels)   {
                cout << "  " << label << ", " << numPostsWithLabel[label] 
                << " examples, log-prior = " 
                << log((double) numPostsWithLabel[label]/numPosts) << endl;
            }
            cout << "classifier parameters:" << endl;
            for (auto lw : allPairs)  {
                cout << "  " << lw.first << ":" << lw.second << ", count = " 
                << numPostsLabelAndWord[lw] << ", log-likelihood = "
                << log((double) numPostsLabelAndWord[lw]
                        /numPostsWithLabel[lw.first]) << endl;
            }
            cout << endl;
        }

        using Pair_type = std::pair<string, string>;

        int numPosts;
        set<string> allLabels;
        set<string> allWords;
        int numWords; //unique words
        map<string, int> numPostsWithWord; //number of posts with a specified word
        map<string, int> numPostsWithLabel; //number of posts of a certain label
        map<Pair_type, int> numPostsLabelAndWord; 
        set<Pair_type> allPairs;
        // EFFECTS: Return a set of unique whitespace delimited words.x
        set<string> unique_words(const string &str) {
            istringstream source(str);
            set<string> words;
            string word;
            while (source >> word) {
                words.insert(word);
            }
            return words;
        }
};



int main(int argc, const char * argv[]) {
    cout.precision(3);
    bool debug = false;

    if (argc > 4 || argc < 3) {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return -1;
  }
    if (argc == 4)  {
        string str = argv[3];
        if (str != "--debug")  {
            cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
            return -1;
        }
        debug = true;
    }

    Classifier cl;
    try
    {
        cl.train(argv[1], debug);
    }
    catch(const csvstream_exception& e)
    {
        cout << "Error opening file: " << argv[1] << endl;
        return -1;
    }
    
    
    try
    {
        cl.test(argv[2]);
    }
    catch(const csvstream_exception& e)
    {
        cout << "Error opening file: " << argv[2] << endl;
        return -1;
    }


    return 0;
}
