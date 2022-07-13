#include <iostream>
#include <bits/stdc++.h>
#include "rapidxml.hpp"

using namespace std;
using namespace rapidxml;

const int TOTALNODE = 4;  // Total Nodes 
string const FILENAME = "13Leases.xml"; //Input file 


struct leaseGroup
{
    vector<vector<string>> leases;
};
struct leaseST
{
    int pID;
    string startingTime;
    string endTime;
};

struct Node
{
    vector<leaseST> leaseAllocated;

    void print()
    {
        for (auto curr : leaseAllocated)
        {
            cout << "pID = " << curr.pID << ", "
                 << "ST = " << curr.startingTime << ", "
                 << "ET = " << curr.endTime << endl;
        }
    }
};

const bool leaseComparatorST(vector<string> const &A, vector<string> const &B)
{
    // let index 2 contain start time, so if A's start time is less than that of B
    // return true
    if (A[2] < B[2])
        return true;
    else
        return false;
}
const bool leaseComparatorAT(vector<string> const &A, vector<string> const &B)
{
    // let index 2 contain alert time, so if A's alert time is less than that of B
    // return true
    if (A[6] < B[6])
        return true;
    else if (A[6] == B[6])
    {
        return A[5] > B[5];
    }
    else
        return false;
}

void displayLeases(vector<vector<string>> const &leases)
{
    for (auto lease : leases)
    {
        for (auto y : lease)
        {
            cout << y << "    ";
        }
        cout << "\n";
    }
}

void calculateAlertTime(vector<vector<string>> &leases)
{
    // alert time = Deadline - Period/Duration
    vector<string> alertTimes;
    for (auto lease : leases)
    {
        string duration = lease[3];
        string deadline = lease[4];

        // finding their difference

        // mainly at 0,1,3,4
        string hr_deadline, hr_duration, min_deadline = "", min_duration = "";
        hr_deadline.push_back(deadline[0]);
        hr_deadline.push_back(deadline[1]);
        hr_duration.push_back(duration[0]);
        hr_duration.push_back(duration[1]);

        min_deadline.push_back(deadline[3]);
        min_deadline.push_back(deadline[4]);

        min_duration.push_back(duration[3]);
        min_duration.push_back(duration[4]);

        // cout<<hr_deadline<<" "<<hr_duration<<" "<<min_deadline<<" "<<min_duration<<"\n";
        int min_deadline_value = stoi(min_deadline), min_duration_value = stoi(min_duration), hr_deadline_value = stoi(hr_deadline), hr_duration_value = stoi(hr_duration);
        int alert_min = min_deadline_value - min_duration_value;
        int alert_hr = hr_deadline_value - hr_duration_value;

        if (alert_min < 0)
        {
            alert_min += 60;
            alert_hr -= 1;
        }
        // preparing the time string again
        string alert_time = "";
        if (alert_hr < 10)
            alert_time += "0";

        alert_time += to_string(alert_hr);
        alert_time += ":";
        if (alert_min < 10)
            alert_time += "0";

        alert_time += to_string(alert_min);
        alert_time += ":00.00";

        // int alertTime = lease[3] - lease[4];
        alertTimes.push_back(alert_time);
    }

    for (int i = 0; i < alertTimes.size(); i++)
        leases[i].push_back(alertTimes[i]);

    // now each of the leases will have alert time
}

vector<leaseGroup> formGroups(vector<vector<string>> const &leases)
{
    vector<leaseGroup> groups;
    leaseGroup running;
    running.leases.push_back(*leases.begin());
    for (int curr = 1; curr < leases.size(); curr++)
    {
        if (leases[curr][2] == leases[curr - 1][2])
        {
            running.leases.push_back(leases[curr]);
        }
        else
        {
            groups.push_back(running);
            running.leases.clear();
            running.leases.push_back(leases[curr]);
        }
    }
    if (running.leases.size() > 0)
    {
        groups.push_back(running);
        running.leases.clear();
    }
    return groups;
}

void swapping()
{
}

bool isDigit(const char &c)
{
    return c - '0' >= 0 && c - '0' <= 9;
}
string maxTime(string const &stForNewlease, string const &nodeFreeTime)
{
    // return maximum time amoung arguments
    int len = stForNewlease.length();
    for (int curr = 0; curr < len; curr++)
    {
        if (isDigit(stForNewlease[curr]) && isDigit(nodeFreeTime[curr]))
        {
            if (stForNewlease[curr] - '0' < nodeFreeTime[curr] - '0')
                return nodeFreeTime;
            else if (stForNewlease[curr] - '0' > nodeFreeTime[curr] - '0')
                return stForNewlease;
        }
    }
    return stForNewlease;
}

bool check(string const &stForNewlease, string const &alertTime)
{
    // stForNewlease is less than or equal to alertTime
    int len = stForNewlease.length();
    for (int curr = 0; curr < len; curr++)
    {
        if (isDigit(stForNewlease[curr]) && isDigit(alertTime[curr]))
        {
            if (stForNewlease[curr] - '0' < alertTime[curr] - '0')
                return true;
            else if (stForNewlease[curr] - '0' > alertTime[curr] - '0')
                return false;
        }
    }
    return true;
}

string addTime(string const &time, string const &duration)
{
    // add these two times in 24-hr format
    cout<<time<<" "<<duration<<endl;
    int len = time.length();
    int carry = 0;
    string fTime;
    for (int curr = len - 2; curr >= 0; curr -= 3)
    {
        int digit = stoi(time.substr(curr, 2));
        int adder = stoi(duration.substr(curr, 2));
        int newT = digit + adder + carry;
        cout<<digit<<" "<<adder<<endl;
        string newTime;
        if (curr == 0)
        {
            newTime = to_string((digit + adder + carry) % 24);
            carry = (digit + adder + carry) / 24;
        }
        else
        {
            newTime = to_string((digit + adder + carry) % 60);
            carry = (digit + adder + carry) / 60;
        }
        reverse(newTime.begin(), newTime.end());
        for (auto i : newTime)
            fTime.push_back(i);
        if (newTime.length() == 1)
            fTime.push_back('0');

        if (curr != 0)
            fTime.push_back(time[curr - 1]);

    }
    reverse(fTime.begin(), fTime.end());
    cout<<fTime<<endl;
    return fTime;
}

bool isLess(string const &time1, string const &time2)
{
    // return true if time1 is greator than or equal to time2
    return check(time2, time1);
}

void allocate(vector<vector<string>> &leases)
{
    // step1 --> sort the leases based on start time
    sort(leases.begin(), leases.end(), leaseComparatorST);
    // step2 --> calculate alert time for all the leases
    calculateAlertTime(leases);
    // step3 --> forming groups accordng to startTimes
    vector<leaseGroup> groups = formGroups(leases);

    vector<vector<string>> rejectedLeases;

    displayLeases(leases);

    // step4 --> Allot based on minimum alert time
    for (auto group : groups)
    {
        sort(group.leases.begin(), group.leases.end(), leaseComparatorAT);
    }

    vector<Node> allNode(TOTALNODE);
    for (auto group : groups)
    {
        for (auto lease : group.leases)
        {
            unordered_set<int> freeNode;
            string startingTimeForNewlease = "24:00:00.00"; // suitable initilization
            for (int mask = 0; mask < TOTALNODE << 1; mask++)
            {

                if (__builtin_popcount(mask) < stoi(lease[5]))
                    continue;

                string tempStartingTimeForNewlease = "00:00:00.00"; // suitable initilization
                unordered_set<int> tempFreeNode;

                for (int node = 0; node < TOTALNODE; node++)
                {
                    if (mask & (node << 1))
                    {
                        if (allNode[node].leaseAllocated.size() == 0)
                        {
                            tempFreeNode.insert(node);
                            tempStartingTimeForNewlease = lease[2];
                            continue;
                        }
                        tempStartingTimeForNewlease = maxTime(tempStartingTimeForNewlease, allNode[node].leaseAllocated.back().endTime);
                        tempFreeNode.insert(node);
                    }
                }
                if(tempStartingTimeForNewlease == "00:00:00.00") continue;
                if (check(tempStartingTimeForNewlease, lease[6]) and isLess(startingTimeForNewlease, tempStartingTimeForNewlease))
                {

                    startingTimeForNewlease = tempStartingTimeForNewlease;
                    freeNode = tempFreeNode;

                }
                // cout<<startingTimeForNewlease<<endl;
            }

            if (freeNode.size() == 0)
            {
                rejectedLeases.push_back(lease);
            }
            else
            {
                leaseST curr;
                curr.pID = stoi(lease[0]);
                curr.startingTime = startingTimeForNewlease;
                curr.endTime = addTime(startingTimeForNewlease, lease[3]);
                for (auto node : freeNode)
                {
                    allNode[node].leaseAllocated.push_back(curr);
                }
            }
        }
    }
    // step5 --> perform swapping

    // step6 --> reject lease

    int currNode = 1;

    for (auto curr : allNode)
    {
        cout << "Node = " << currNode << endl;
        curr.print();
        currNode++;
    }

    cout<<"Rejected Leases = "<<rejectedLeases.size()<<endl;
    cout<<"Accepted Leases = "<<leases.size() - rejectedLeases.size();

}

vector<vector<string>> pareseXML()
{
    xml_document<> doc;
    xml_node<> *root_node = NULL;
    vector<vector<string>> leases;
    ifstream theFile(FILENAME);
    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');

    // Parse the buffer
    doc.parse<0>(&buffer[0]);

    // Find out the root node
    root_node = doc.first_node("lease-workload");

    // Parsing the XML File

    for (xml_node<> *lease_requests_node = root_node->first_node("lease-requests"); lease_requests_node; lease_requests_node = lease_requests_node->next_sibling())
    {
        for (xml_node<> *lease_request = lease_requests_node->first_node("lease-request"); lease_request; lease_request = lease_request->next_sibling())
        {

            vector<string> lease_record;
            string lease_arrival_time = lease_request->first_attribute("arrival")->value();
            // Lease Block
            for (xml_node<> *lease_info = lease_request->first_node("lease"); lease_info; lease_info = lease_info->next_sibling())
            {
                // determining lease id
                // int leaseId = stoi(lease_info->first_attribute("id")->value());
                string leaseId = lease_info->first_attribute("id")->value();

                // determing lease stats
                xml_node<> *lease_node_inner = lease_info->first_node("nodes");
                xml_node<> *lease_node_num_clusters = lease_node_inner->first_node("node-set");

                // int numberOfNodesRequired = stoi(lease_node_num_clusters->first_attribute("numnodes")->value());
                string numberOfNodesRequired = lease_node_num_clusters->first_attribute("numnodes")->value();

                // determing start time
                xml_node<> *start_time_node = lease_info->first_node("start");

                xml_node<> *exact_start_time = start_time_node->first_node("exact");
                string lease_start_time = exact_start_time->first_attribute("time")->value();

                // determing duration
                xml_node<> *duration_node = lease_info->first_node("duration");
                string lease_duration = duration_node->first_attribute("time")->value();

                // determing start time
                xml_node<> *deadline_node = lease_info->first_node("deadline");
                string lease_deadline = deadline_node->first_attribute("time")->value();

                // storing the values in lease record;
                lease_record.push_back(leaseId);
                lease_record.push_back(lease_arrival_time);
                lease_record.push_back(lease_start_time);
                lease_record.push_back(lease_duration);
                lease_record.push_back(lease_deadline);
                lease_record.push_back(numberOfNodesRequired);
            }

            leases.push_back(lease_record);
        }
    }
    return leases;
}

int main()
{
    // parsing
    vector<vector<string>> leases = pareseXML();
    allocate(leases);
    return 0;
}