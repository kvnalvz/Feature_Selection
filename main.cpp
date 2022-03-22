
// Kevin Alvarez
// NetID: kalva050
// SID: 862131623
// CS170 Spring 2021
// Professor Montazeri
// June 13, 2021
// Project 2, part 3

// Project 2 

// To compile: g++ -std=c++11


#include <iostream>
#include <string>
#include <cmath>
#include <queue>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

deque<int> subsets;
int rows = 0;
int cols = 0;
deque<int> featureSet;
double highestVal = 0;
double peakHigh = 0;
deque<int> chosenSub;
deque<int> finalSub;



class Entry
{
public:
    int classID;
    vector<double> featuress;
    
};


class Classifier
{
public:
    deque<Entry> instas;
    int test(Classifier, Classifier);
    
};

class Validator
{
public:
    void leaveOneOut(deque<int>);
};



Entry k;
Classifier originalData;
Classifier normalizeData;
Validator done;



int Classifier::test(Classifier t, Classifier p)
{
    double euclideanD = 0;
    double shortestDis = 10000;
    int id = 0;
   
          
        // This function will basically guess what class the instance that is being tested belongs to. The tested instance is "temp" in leaveOneOut function
        // It will make it choose based on the euclidean distance (nearest neighbor) 
        // It will go through all the feature # in the feature set
        // The formula is SQUARE-ROOT((instance1's feature 1 value - temp's feature 1 value)^2+(instance1's feature 2 value - temp's feature 2 value)^2+...)
        // General idea^ not exact but yeah, will test every instance in the training set with temp
        for(int j = 0; j < rows-1; j++)
        {
            for(int i = 0; i < subsets.size(); i++)
            {
                int num = subsets.at(i);
                euclideanD += pow(p.instas[j].featuress[num-1] - t.instas[0].featuress[num-1],2);
                   
            }
            euclideanD = sqrt(euclideanD);         // after an instance got tested with temp  
                
            
            if(euclideanD <= shortestDis)       // if that value^ is the shortest then we keep track of that instance's class ID
            {
                shortestDis = euclideanD;
                id = p.instas[j].classID;
            }
            euclideanD = 0;
        }
            
    
    
    return id;  // we return the id of the instance that has the shortest distance with the temp instance
    
}




void Validator::leaveOneOut(deque<int> sub)
{
    Classifier temp, t;
    int correct = 0;
    int num = 0;
    double finalAcc = 0;

    
    // So basically what happens below is it removes on instance from the training set and we put in a new object called temp
    // Then we run the test function, where we test that removed instance on the training data
    // test function returns a number, which is the number of the class that test function guessed (1 or 2)
    // Then it compares that return number with the removed instances(temp) actual class number
    // if they are the same we increment correct, if not then we don't
    // then we add that removed instance(temp) back into the training set and we start the loop again, but now we pick a new instance to remove
    for(int i = 0; i < rows; i++)
    {
        temp.instas.push_back(normalizeData.instas.at(i));
        normalizeData.instas.erase(normalizeData.instas.begin() + i);
        num = t.test(temp, normalizeData);
            
        if(temp.instas[0].classID == num)
        {
            correct += 1;
        }
            
            
        normalizeData.instas.insert(normalizeData.instas.begin()+i, temp.instas[0]);
        temp.instas.clear();
        num = 0;
    }
        
    

    finalAcc = ((double)correct/(double)rows); // will be the percent of how many time it guessed the correct class for the instances
    
    if(finalAcc > highestVal)   // if the percent^ is higher than the previous feature set we tested then we initialize that value as the highest percent
    {                           // up till that point
        highestVal = finalAcc;
        chosenSub = sub;    // if this feature set has the highest percent, then this feature set is the "chosenSub" feature that has the highest percent
    }                       // up till that point
    

    cout << (double)(finalAcc*100) << "%" << endl;
    
   // exit(0);
}





bool checker(int i)
{
    bool k = false;
    
    // This function works with the Forward Selection function, if the feature that is passed is already in the feature set then it returns true
    if(!featureSet.empty())
    {
        deque <int> :: iterator it;
        
            for (it = featureSet.begin(); it != featureSet.end(); ++it)
            {
                if(*it == i)
                {
                    k = true;
                    break;
                }
            }
        

    }
    
    return k;
}


void printChosen()  // Will basically just print out what values are in the featureSet container
{                   // this continaer has all the feature # that are in the feature set

    if(!featureSet.empty())
    {
        deque <int> :: iterator it;
        
        for (it = featureSet.begin(); it != featureSet.end(); ++it)
        {
            cout << *it << ",";
        }


    }
    
}




void forwardSelection()
{
    bool quit = true; // for continuing the do while loop
    int counter = 0;    // for checking for local maxima, its the number of times the accuracy decreased
    
    
    do{
    
        for(int i = 0; i < cols; i++)
        {
            if(checker(i+1) == false) // if true that means this feature # is already in the feature set
            {
                subsets = featureSet; // adds whats in the featureSet container already
                subsets.push_back(i+1); // and it adds an additional feature # that will be tested with the previous feature set
                cout << "Using feature(s) {"; // for example, if previous the feature set is {3}, then it will test {3,1} then {3,2} then {3,4}. . . etc
                printChosen();
                cout << i+1 << "} accuracy is ";
                done.leaveOneOut(subsets);
                subsets.clear(); // clears the subsets container, because each loop it tests it by adding only one feature to subsets 
            }                   // example, featureSet={3}, then one loop subsets = {3,1}, then next loop subsets = {3,2}...etc
        
        }
        featureSet = chosenSub; 
    
    
        cout << "\nFeature set {";
        for(int i = 0; i < featureSet.size(); i++)
        {
            cout << featureSet.at(i) << ",";
        }
        cout << "} was the best, accuracy is " << highestVal*100 << "%\n\n";
        // Prints the feature set
        
        
        if((highestVal) >= peakHigh)
        {
            peakHigh = highestVal;      // the accuracy we got from this iteration was the highest accuracy
            quit = false;               // for continuing the do while loop
            finalSub = featureSet;     // the final feature set
            counter = 0;                // 0 because the accuracy didn't decrease
        }
        else
        {
            counter++;
            if(counter < 2) // if accuracy has only decreased once so far, checks for local maxima
            {
                cout << "(Warning! Accuracy has decreased! Continuing search in case of local maxima)\n\n";
                quit = false;
            }
            else // accuracy has decrease more than once
            {
                quit = true; // stop do while loop
            }
        }

        highestVal = 0;
    }while(quit == false);
    
    

    featureSet = finalSub; // featureSet gets initialize finalSub from above^
    cout << "\n(WARNING! Accuracy has decreased!)\n";
    cout << "Finished search!!! The best feature subset is {";
    printChosen();
    cout << "}, which has an accuracy of " << peakHigh*100 << "%\n\n";;
    return;
    // Print final feature set
    
    
}




void backwardElmination()
{
    bool quit = true; // will check if we continue doing this algorithm
    int counter = 0; // counter to keep track of how many times the feature set % decreases
    
    for(int i = 0; i < cols; i++)
    {
        subsets.push_back(i+1);     // adds the feature #s into this deque, not the feature's value. Ex. subsets= {1,2,3,...,10}
    
    }
    featureSet = subsets;  // since we start with a full feature set, the featureSet deque has all feature #s. 
    // featureSet contains all the feature #s at this point. featureSet = {1,2,3,...,10}

    cout << "Using feature(s) {";
    printChosen(); // Prints the feature #s that are in the featureSet container
    cout << "} accuracy is ";
    done.leaveOneOut(subsets);  // runs the leave one out function. Pass in the subsets container, that currently has all the feature #s
    
    
    cout << "\nFeature set {";
    for(int i = 0; i < featureSet.size(); i++) // at thing point there has been one feature deleted from the featureSet container
    {                                           // it will print the feature #s it now contains in the container at this point
        cout << featureSet.at(i) << ",";
    }
    cout << "} was the best, accuracy is " << highestVal*100 << "%\n\n";
    peakHigh = highestVal;
    highestVal = 0;
    
    
    do{
        
        // you can think of this do while loop when it first starts as the second iteration. what came before was testing
        // a full feature set with all the feature #s
        // Now when this starts the there has been one feature deleted and we will continue to delete more feature # from the deque

        subsets = featureSet; // assign subsets the feature numbers in the featureSet
    
        for(int i = 0; i < subsets.size(); i++)
        {

            
            int temp;
            int temp2;
        
            temp = subsets.at(i);
            temp2 = featureSet.at(i);
        
            subsets.erase(subsets.begin()+i);   // will erase one feature # from subsets
            featureSet.erase(featureSet.begin()+i); // will erase one feature # from this as well to print which values will be tested
            cout << "Using features(s) {";
            printChosen(); // prints values in featureSet
            featureSet.insert(featureSet.begin()+i, temp2);
            cout << "} accuracy is ";
            done.leaveOneOut(subsets); // subsets is passed in because subsets is what will be tested
            subsets.insert(subsets.begin()+i, temp);
        
        }
    
        featureSet = chosenSub; 
    
    
        cout << "\nFeature set {";
        for(int i = 0; i < featureSet.size(); i++)
        {
            cout << featureSet.at(i) << ",";
        }
        cout << "} was the best, accuracy is " << highestVal*100 << "%\n\n";
        // prints the feature set with the highest accuracy
        
        
        if((highestVal) >= peakHigh) // if true, then we will continue for another loop
        {
            peakHigh = highestVal; // the accuracy we get from this iteration is the highest accuracy up to till this point
            quit = false; // for continuing the do while loop
            finalSub = featureSet; // the final feature set
            counter = 0; // 0 because the accuracy did not decrease this iteration
        }
        else
        {
            counter++; // if the accuracy was lower than previous accuracy
            if(counter < 2) // if less than 2 we check for local maxima
            {
                cout << "(Warning! Accuracy has decreased! Continuing search in case of local maxima)\n\n";
                quit = false; // we continue another loop
            }
            else
            {
                quit = true; // if the accuracy has decrease again, then we stop this algorithm
            }
            
        }

        highestVal = 0;
    
    
    }while(quit == false);
    
    
    
    featureSet = finalSub; // did this so we can use the printChosen() function
    cout << "\n(WARNING! Accuracy has decreased!)\n";
    cout << "Finished search!!! The best feature subset is {";
    printChosen();
    cout << "}, which has an accuracy of " << peakHigh*100 << "%\n\n";;
    return;
    
    
}



// Normalizes the dataset
double normalize(double feature, double number)
{
    double num = 0;
    double sum = 0;
    double mean = 0;
    double stdBefore = 0;
    double stdAfter = 0;
    
    
    for(int i = 0; i < rows; i++)
    {
        sum += originalData.instas[i].featuress[feature]; // sum of every instance's same feature value
    }                                                       // for example, we get the sum of every instances feature 2 value

    mean = sum / rows; // self explained
    
        
    for(int i = 0; i < rows; i++)
    {

        stdBefore += pow(originalData.instas[i].featuress[feature] - mean,2.0); 
            
    }
    stdBefore = (double)(stdBefore/rows);
    stdAfter = sqrt(stdBefore);
    
    // ^^^ basically square root((original feature 1 value - mean)^2+(original feature 2 value - mean)^2+....)


        
    num = (number - mean) / stdAfter; // self explained, (original feature # value - mean / standard deviation)
        
    
    return num; // return that value ^
}




int main()
{
    
    //int defaultRate = 0;
    string fileName;
    int algorithmChoice;
    cout << "Welcome to Kevin Alvarez's Feature Selection Algorithm.\n\n";
    cout << "Type in the name of the file to test. (cs_170_large3.txt or cs_170_small3.txt) ?\n";
    cout << "Choice: ";
    cin >> fileName;
    
    cout << "\nType in the number of the algorithm you want to run.\n";
    cout << "1) Forward Selection\n2) Backward Elimination\n";
    cout << "Choice: ";
    cin >> algorithmChoice;
    

    int featureCheck = 0;
    
    
    
    ifstream inFile;
    
    
    cout << "\nLoading dataset...\n";
    

   
    inFile.open(fileName);
    
    if(!inFile.is_open())
    {
        cout << "ERROR" << endl;
        exit(-1);
    }
    
    
    
    int ct = 0; // short for "count"
    string buffer;

    // using a similiar method that a classmate provided on piazza, post @132
    // This is used to read from the dataset
    while(getline(inFile,buffer))
    {
        
        stringstream line(buffer);
        int classifier;
        line >> classifier;
        
        k.classID = classifier; // the classifier is either 1 or 2, representing the instance's correct class
        
        double feature;
        
        rows++; // number of rows = number of instances
        
        ct = 0;
        while(line >> feature)
        {
            if(ct == 0)
            {
                ct = 1;
                continue;
            }
            k.featuress.push_back(feature); // adds the feature value to the object k's featuress vector
            
            if(featureCheck == 0)
            {
                cols++; // number of cols = number of features
            }
            

        }
        if(featureCheck == 0) // after it's counted the number of features for the first instance, it stops counting them
        {                       // because every instance has the same number of features
            featureCheck = 1;
        }
        
        originalData.instas.push_back(k); // you can think of the object k as an instance, therefore we push an instance into the instas deque
        k.featuress.clear();    // we clear k because the next iterator will be a new instance and feature values
        
    }
    
    normalizeData = originalData; // just wanted to initialize normalizeData with something, probably didn't need to do this.
    
    cout << "This dataset has " << cols << " features, with " << rows << " instances.\n\n";

    
    
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++) // the new values of each feature after normalization will be given to normalizeData object
        {
            normalizeData.instas[i].featuress[j] = normalize(j, originalData.instas[i].featuress[j]); 
        }
    }
    
    cout << "Please wait while I normalize the data. . . DONE!\n\n";
    
    // The deafult rate, basically will just guess what class each instance belongs to

    //----
    //Validator defaultTest;
    //defaultRate = (rand() % cols) + 1;
    //subsets.push_back(defaultRate);
    cout << "Running nearest neighbor with no features (default rate), using \"leave-one-out\" evaluation, ";
    cout << "I get an accuracy of ";
    done.leaveOneOut(subsets);
    cout << endl;
    subsets.clear();
    chosenSub.clear();
    featureSet.clear();
    peakHigh = 0;
    highestVal = 0; // clears everything before we start the actual two algorithms
    
    //------
    
    
    
    cout << "Beginning search.\n\n";
    
    
    if(algorithmChoice == 1)
    {

        forwardSelection();
        
    }
    
    else if(algorithmChoice == 2)
    {

        backwardElmination();
        
    }
    
    inFile.close();
    
    return 0;
}
