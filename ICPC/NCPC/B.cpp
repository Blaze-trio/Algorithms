#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
using namespace std;

int main() {
    int n;
    cin >> n;
    for(int i = 0; i < n; i++){
        string x;
        cin >> x;
        try{
            double y = stod(x);
            if(y == 0.0){
                cout << "inf" << endl;
            }else{
                cout << 1.0 / y << endl;
            }
        }catch(...){
            if(x == "nan" || x == "NaN"){
                cout << "NaN" << endl;
            }else if(x == "inf" || x == "+inf" || x == "INF" || x == "+INF"){
                cout << "0" << endl;
            }else if(x == "-inf" || x == "-INF"){
                cout << "-0" << endl;
            }else if(x == "-0" || x == "+0"){
                cout << "inf" << endl;
            }else{
                cout << "NaN" << endl;
            }
        }
    }
    return 0;
}