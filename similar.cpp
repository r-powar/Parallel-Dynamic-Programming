// CPSC 5600 - String Similarity
//
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

#include "tbb/tbb.h"
using namespace tbb;

bool isVowel(char word){
    return word == 'a' || word == 'e' || word == 'i' || word == 'o' || word == 'u';
}

int calculateMismatch(char first, char second){
    int cost;
    bool checkFirstVowel = isVowel(first);
    bool checkSecondVowel = isVowel(second);
    
    if((first != second) && checkFirstVowel && !checkSecondVowel){
        cost = 3;
    }else if((first != second) && !checkFirstVowel && checkSecondVowel){
        cost = 3;
    }else if(checkFirstVowel && checkSecondVowel && (first != second)){
        cost = 1;
    }else{
        cost = 1;
    }
    
    return cost;
}
             
int getMinimum(int val1 , int val2, int val3){
    
    if((val1 <= val2) && (val1 <= val3)){
        return val1;
    }
    
    if((val2 <= val1) && (val2 <= val3)){
        return val2;
    }
    
    if((val3 <= val1) && (val3 <= val2)){
        return val3;
    }
    
    return 0;
    
}

int calculatingScore(string str1, string str2){
    
    int sizestr1 = str1.size();
    int sizestr2 = str2.size();
    
    vector<int> initial(sizestr2 + 1);
    vector<vector<int> > result(sizestr1 + 1, initial);
    
    result[0][0] = 0;
    
    for(int i = 1; i < sizestr1 + 1; i++){
        result[i][0] = i * 2;
    }
    
    for(int j = 1; j < sizestr2 + 1; j++){
        result[0][j] = j * 2;
    }
    
    
    for(int k = 1; k <= sizestr1; k++){
        parallel_for(blocked_range<int>(1, k + 1),
                     [&](const blocked_range<int> &r){
                         for(int row = r.begin(); row  < r.end(); row++){
                             int cost;
                             
                             int col = k - row + 1;
                             if(col > sizestr2){
                                 break;
                             }
                             if(str1[row - 1] == str2[col - 1]){
                                    cost = 0;
                             }else{
                                    cost = calculateMismatch(str1[row - 1], str2[col - 1]);
                             }
                             
                             int val1 = result[row - 1][col] + 2;
                             int val2 = result[row][col-1] + 2;
                             int val3 = result[row-1][col-1] + cost;
                                         
                            result[row][col] = getMinimum(val1, val2, val3);
                         }
                     }
        
        );
    }
    
    
    for(int k = 1; k <= sizestr2; k++){
        int row = sizestr1;
        for(int col = k; (col <= sizestr2); col++){
            int cost;
            
            if(row < 1){
                break;
            }
            
            if(str1[row - 1] == str2[col - 1]){
                cost = 0;
            }else{
                cost = calculateMismatch(str1[row - 1], str2[col - 1]);
            }
            
            int val1 = result[row - 1][col] + 2;
            int val2 = result[row][col-1] + 2;
            int val3 = result[row-1][col-1] + cost;
            
            result[row][col] = getMinimum(val1, val2, val3);
            row--;
            
        }
    }
    
    
    int finalResult = result[sizestr1][sizestr2];
    
    return finalResult;
    
}

int main(int argc, char *argv[])
{
    int answer;
    string firststr;
    string secondstr;

  // Check for invalid command lines
  if (argc != 2) {
    cerr << "Invalid command line - usage: <input file>" << endl;
    exit(-1);
  }
  
    ifstream ifile(argv[1]);
    ifile >> firststr;
    ifile >> secondstr;


  // Start the timer
  tick_count start_time = tick_count::now();

  // TODO: Perform the algorithm
  answer = calculatingScore(firststr, secondstr);

  // Stop the timer
  tick_count end_time = tick_count::now();
  double run_time = (end_time - start_time).seconds();

  // Print the results
  cout << "Answer: " << answer << endl;
  cout << "Time:   " << run_time << endl;

  return 0;
}
