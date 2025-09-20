#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>

struct Job {
    int weight;
    int length;
    int difference;
    double ratio;
    
    Job(int w, int l) : weight(w), length(l), difference(w - l), ratio((double)w / l) {}
};

bool compareJobs(const Job& a, const Job& b) {
    return a.ratio > b.ratio;
}

int main() {
    const char* filename = "jobs.txt";
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return 1;
    }
    
    int numJobs;
    file >> numJobs;
    
    std::vector<Job> jobs;
    jobs.reserve(numJobs);
    
    for (int i = 0; i < numJobs; i++) {
        int weight, length;
        file >> weight >> length;
        jobs.emplace_back(weight, length);
    }
    
    file.close();
    std::sort(jobs.begin(), jobs.end(), compareJobs);
    
    long long totalWeightedCompletionTime = 0;
    long long currentTime = 0;
    
    for (const Job& job : jobs) {
        currentTime += job.length;
        totalWeightedCompletionTime += (long long)job.weight * currentTime;
    }
    
    std::cout << "Sum of weighted completion times: " << totalWeightedCompletionTime << std::endl;
    
    return 0;
}