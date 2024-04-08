#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
using namespace std;

class DFA
{
	public:
		vector<string> states;
        vector<string> transition;
		bool flag;
        string statediff;

        DFA()
        {
            flag = false;
        }

		DFA(int size)
		{
			states.resize(size);
			flag = false;
		}
};

bool nameandflag(const pair<string, DFA> &a, const pair<string, DFA> &b)
{
    if (a.second.flag != b.second.flag) 
    {
        return !a.second.flag && b.second.flag;
    } 
    else if(a.second.statediff != b.second.statediff)
    {
        return a.second.statediff < b.second.statediff;
    }
    else
    {
        return a.first < b.first;
    }
}

string roman(int num)
{
    string ans;
    
    while(num != 0)
    {
        if(num % 10 == 0)
        {
            ans += "X";

            num -= 10;
        }
        else if((num + 1) % 10 == 0)
        {
            ans += "IX";

            num -= 9;
        }
        else if((num + 1) % 5 == 0)
        {
            ans += "IV";

            num -= 4;
        }
        else if(num % 5 == 0)
        {
            ans += "V";

            num -= 5;
        }
        else
        {
            ans += "I";
            num -= 1;
        }
    }
    return ans;
}

void printingtranstions(vector<pair<string, DFA>> mini)
{
    for(const auto & pl : mini)
    {
        cout << "Transitions for " << pl.first << endl;

        for(const auto it : pl.second.transition)
        {
            cout << it << ", ";

        }
        cout << endl;
    }
}

string forreturnstring(vector<pair<string, DFA>> mini)
{
    string ans;

    for(const auto & pl : mini)
    {
        for(const auto it : pl.second.transition)
        {
            ans += it;

            ans += ", ";
        }
    }
    return ans;
}

pair<string, DFA> find(string f, vector<pair<string, DFA>> m)
{
    for(const auto fin : m)
    {
        if(f == fin.first)
        {
            return fin;
        }
    }

    return {};
}

int statecount = 2;

int main()
{
    int numberofstates, alphabets;

    cout << "Enter the number of states: " << endl;

    cin >> numberofstates;

    cout << "Enter the number of possibilities: " << endl;

    cin >> alphabets;

    vector <string> set(alphabets);

    for(int i = 0; i < alphabets; i++)
    {
        cout << "Enter " << i + 1 << " input: ";

        cin >> set[i];
    }

    cout << "Enter the number of final states: ";

    int finalstates;

    cin >> finalstates;

    vector <string> finals(finalstates);

    for(int i = 0; i < finalstates; i++)
    {
        cout << "Enter the " << i + 1 << "th state: ";

        cin >>finals[i];
    }

    unordered_map <string, DFA> minimization;

    vector<string> temp(alphabets);

    for(int i = 0; i < numberofstates; i++)
    {
        minimization["q" + to_string(i)] = DFA(alphabets);
    }

    // Input:

    for(int j = 0; j < numberofstates; j++)
    {
        minimization["q" + to_string(j)] = DFA(alphabets);

        cout << "Enter outputs for q" << j << ": " << endl;

        for(int i = 0; i < alphabets; i++)
        {
            cout << "Enter for " << set[i] << ": " << endl;

            cin >> minimization["q" + to_string(j)].states[i];
        }
    }
    // Final State Allocation:

    for(int i = 0; i < numberofstates; i++)
    {
        for(int j = 0; j < finalstates; j++)
        {
            if(finals[j] == ("q" + to_string(i)))
            {
                minimization["q" + to_string(i)].flag = true;
            }
        }
    }

    for(int i = 0; i < numberofstates; i++)
    {
        if(minimization["q" + to_string(i)].flag)
        {
            minimization["q" + to_string(i)].statediff = roman(2);
        }
        else
        {
            minimization["q" + to_string(i)].statediff = roman(1);
        }
    }

    // Minimization:

    unordered_map<string, DFA> tempmap;

    for(auto & pair : minimization)
    {
        for(int j = 0; j < set.size(); j++)
        {
            string t;

            t = pair.second.states[j];

            auto find = minimization.find(t);

            pair.second.transition.push_back(find->second.statediff);
        }
    }

    vector<pair<string, DFA>> mini(minimization.begin(), minimization.end());

    // Sorting with respect to names and non-accepting and accepting states

    sort(mini.begin(), mini.end(), nameandflag);

    string prev = forreturnstring(mini);

    string curr;

    pair <string, DFA> search;

    cout << "Initial Transtion Table:" << endl;

    printingtranstions(mini);

    cout << endl;

    while(prev != curr)
    {
        // For Non-Accepting States:

        for(int i = 0; i < mini.size(); i++)
        {
            if(mini[i].second.statediff == mini[i + 1].second.statediff)
            {
                if(mini[i].second.flag == false && mini[i + 1].second.flag == false)
                {
                    if(mini[i].second.transition == mini[i + 1].second.transition)
                    {
                        continue;
                    }
                    else
                    {
                        statecount++;

                        int temp = i + 1;

                        mini[i + 1].second.statediff = roman(statecount);

                        while(mini[temp].second.transition == mini[temp + 1].second.transition)
                        {
                            if(mini[temp].second.flag == mini[temp + 1].second.flag)
                            {
                                mini[temp + 1].second.statediff = roman(statecount);

                                temp++;
                            }
                            else
                            {
                                break;
                            }
                        }  
                        break;
                    }
                }
            }
        } 

        // For Accepting States:

        for(int i = 0; i < mini.size() - 1; i++)
        {
            if(mini[i].second.statediff == mini[i + 1].second.statediff)
            {
                if(mini[i].second.flag == true && mini[i + 1].second.flag == true)
                {
                    if(mini[i].second.transition == mini[i + 1].second.transition)
                    {
                        continue;
                    }
                    else
                    {
                        statecount++;

                        int temp = i + 1;

                        mini[i + 1].second.statediff = roman(statecount);

                        while(mini[temp].second.transition == mini[temp + 1].second.transition)
                        {
                            if(mini[temp].second.flag == mini[temp + 1].second.flag)
                            {
                                mini[temp + 1].second.statediff = roman(statecount);

                                temp++;
                            }
                            else
                            {
                                break;
                            }
                           
                        }
                        break;
                    }
                }
            } 
        }

        for(auto & pair : mini)
        {
            for(int j = 0; j < set.size(); j++)
            {
                string t;

                t = pair.second.states[j];

                search = find(t, mini);

                pair.second.transition[j] = search.second.statediff;
            }
        }

        prev = curr;

        curr = forreturnstring(mini);

        sort(mini.begin(), mini.end(), nameandflag);
    }

    vector<pair<string, vector<string>>> finalprinting;

    pair<string, vector<string>> temppair;

    for(int i = 0; i < mini.size() - 1; i++)
    {
        if(mini[i].second.statediff != mini[i + 1].second.statediff)
        {
            temppair.first = mini[i].second.statediff;

            temppair.second = mini[i].second.transition;

            finalprinting.push_back(temppair);

            temppair = make_pair(string(), vector<string>());   
        }
    }

    temppair.first = mini[mini.size() - 1].second.statediff;

    temppair.second = mini[mini.size() - 1].second.transition;

    finalprinting.push_back(temppair);

    temppair = make_pair(string(), vector<string>());

    cout << "Final Transition Table: " << endl;

    printingtranstions(mini);

    sort(finalprinting.begin(), finalprinting.end());

    cout << endl;

    cout << "Final Transition Table for states: " << endl;

    for(int i = 0; i < finalprinting.size(); i++)
    {
        cout << finalprinting[i].first << ": ";

        for(int j = 0; j < finalprinting[i].second.size(); j++)
        {
            cout << finalprinting[i].second[j] << ", ";
        }
        cout << endl;
    }
}