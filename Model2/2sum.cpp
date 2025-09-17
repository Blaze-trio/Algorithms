#include <iostream>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <chrono>

class TwoSumSolver {
private:
    std::unordered_map<long long, int> numberCount; // Changed to long long for safety
    std::unordered_set<long long> uniqueNumbers;    // Changed to long long for safety
    
public:
    // Load numbers from file with better error checking
    bool loadFromFile(const std::string& filename) {
        std::cout << "Attempting to open file: " << filename << std::endl;
        
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            std::cerr << "Make sure the file exists in the current directory." << std::endl;
            return false;
        }
        
        std::cout << "File opened successfully. Reading numbers..." << std::endl;
        
        long long number;
        int count = 0;
        while (file >> number) {
            numberCount[number]++;
            uniqueNumbers.insert(number);
            count++;
            
            // Show progress every 100000 numbers
            if (count % 100000 == 0) {
                std::cout << "Read " << count << " numbers so far..." << std::endl;
            }
        }
        
        file.close();
        std::cout << "Finished reading file." << std::endl;
        std::cout << "Loaded " << uniqueNumbers.size() << " unique numbers" << std::endl;
        std::cout << "Total numbers: " << count << std::endl;
        
        if (count == 0) {
            std::cerr << "Warning: File appears to be empty or contains no valid numbers!" << std::endl;
            return false;
        }
        
        return true;
    }
    
    // Get total count of all numbers (including duplicates)
    int getTotalCount() const {
        int total = 0;
        for (const auto& pair : numberCount) {
            total += pair.second;
        }
        return total;
    }
    
    // Check if target t can be expressed as x + y where x and y are DISTINCT
    bool canFormTarget(long long target) const {
        for (long long x : uniqueNumbers) {
            long long y = target - x;
            
            if (x != y) {
                // Different values: both must exist in the input
                if (uniqueNumbers.find(y) != uniqueNumbers.end()) {
                    return true;
                }
            }
        }
        return false;
    }
    
    // Optimized approach - generate all valid targets
    int solveTwoSumOptimized() {
        std::unordered_set<long long> validTargets;
        
        std::cout << "Computing valid targets using optimized approach..." << std::endl;
        
        // Convert to vector for easier iteration
        std::vector<long long> numbers(uniqueNumbers.begin(), uniqueNumbers.end());
        std::cout << "Processing " << numbers.size() << " unique numbers..." << std::endl;
        
        // Generate all possible sums with DISTINCT numbers
        for (size_t i = 0; i < numbers.size(); i++) {
            for (size_t j = i + 1; j < numbers.size(); j++) {
                long long x = numbers[i];
                long long y = numbers[j];
                long long target = x + y;
                
                if (target >= -10000 && target <= 10000) {
                    validTargets.insert(target);
                }
            }
            
            // Show progress every 1000 numbers
            if (i % 1000 == 0) {
                std::cout << "Processed " << i << "/" << numbers.size() << " numbers..." << std::endl;
            }
        }
        
        return validTargets.size();
    }
    
    // Show some sample numbers from the file
    void showSampleNumbers(int numSamples = 10) {
        std::cout << "\nSample numbers from file:" << std::endl;
        int count = 0;
        for (long long num : uniqueNumbers) {
            std::cout << num << " ";
            count++;
            if (count >= numSamples) break;
        }
        std::cout << std::endl;
    }
};

int main() {
    std::cout << "2-SUM Problem Solver (Distinct Numbers Only)" << std::endl;
    std::cout << "===========================================" << std::endl;
    
    // Try different possible filenames
    std::vector<std::string> possibleFiles = {"2sum.txt", "2Sum.txt", "2SUM.txt", "algo1-programming_prob-2sum.txt"};
    
    TwoSumSolver solver;
    bool fileLoaded = false;
    
    for (const std::string& filename : possibleFiles) {
        std::cout << "\nTrying filename: " << filename << std::endl;
        if (solver.loadFromFile(filename)) {
            fileLoaded = true;
            break;
        }
    }
    
    if (!fileLoaded) {
        std::cout << "Could not load any file. Please ensure the 2sum data file is in the current directory." << std::endl;
        std::cout << "Expected filename: 2sum.txt" << std::endl;
        return 1;
    }
    
    // Show sample numbers
    solver.showSampleNumbers(10);
    
    // Use the optimized approach
    std::cout << "\nStarting computation..." << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    int result = solver.solveTwoSumOptimized();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "\n=== FINAL RESULT ===" << std::endl;
    std::cout << "Number of valid targets in [-10000, 10000]: " << result << std::endl;
    std::cout << "Execution time: " << duration.count() << " ms" << std::endl;
    
    return 0;
}