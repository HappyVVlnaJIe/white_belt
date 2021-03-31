#include <iostream>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <sstream> 
#include <set>
#include <map>
#include <vector>
using namespace std;

int main() {
    char letter[7] = { 'A','B','C','_','D','E','F' };
    int n, index;
    string str;
    vector<string> mas;
    vector<int> indexs;
    cin >> n;
    getline(cin, str);
    for (int i = 0; i < n; i++)
    {
        getline(cin, str);
        mas.push_back(str);
    }
    int m, num_pass;
    string side, position;
    cin >> m;
    for (int i = 0; i < m; i++)
    {
        cin >> num_pass >> side >> position;
        string result;
        bool have_pos = false;
        for (int j = 0; j < n; j++)
        {
            result = mas[j];
            bool fl = true;
            if (side == "left") {
                if (position == "window") {
                    for (int q = 0; q < num_pass; q++) {
                        if (result[q] == '#') {
                            fl = false;
                            break;
                        }
                        result[q] = 'X';
                        indexs.push_back(q);
                    }
                }
                else {

                    for (int q = 3 - num_pass; q < 3; q++) {
                        if (result[q] == '#') {
                            fl = false;
                            break;
                        }
                        result[q] = 'X';
                        indexs.push_back(q);
                    }
                }
            }
            else {
                if (position == "window") {
                    for (int q = 7 - num_pass; q < 7; q++) {
                        if (result[q] == '#') {
                            fl = false;
                            break;
                        }
                        result[q] = 'X';
                        indexs.push_back(q);
                    }
                }
                else {

                    for (int q = 4; q < num_pass + 4; q++) {
                        if (result[q] == '#') {
                            fl = false;
                            break;
                        }
                        result[q] = 'X';
                        indexs.push_back(q);
                    }
                }
            }
            if (fl) {
                mas[j] = result;
                have_pos = true;
                index = j;
                break;
            }
            else {
                indexs.clear();
            }
        }
        if (!have_pos) {
            cout << "Cannot fulfill passengers requirements" << endl;
        }
        else {
            cout << "Passengers can take seats: ";
            for (int i = 0; i < indexs.size(); i++)
            {
                cout << index + 1 << letter[indexs[i]];
                if (i != indexs.size() - 1) {
                    cout << ' ';
                }
            }
            cout << endl;
            for (int i = 0; i < n; i++)
            {
                cout << mas[i] << endl;
            }
            std::replace(mas[index].begin(), mas[index].end(), 'X', '#');
        }
    }
    return 0;
}