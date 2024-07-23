#include <bits/stdc++.h>
using namespace std;
template <typename T>

class Graph
{
private:
    map<T, list<pair<T, float>>> adjList;

public:
    list<T> path;
    void addEdgeBetween(T u, T v, float distance, bool isBidirex = true)
    {
        adjList[u].push_back(make_pair(v, distance));
        if (isBidirex)
        {
            adjList[v].push_back(make_pair(u, distance));
        }
    }

    void dijkstraGetShortestPathTo(T destination, map<T, T> &prev)
    {
        for (; destination != ""; destination = prev[destination])
        {
            path.push_back(destination);
        }
        path.reverse();
        cout << "\t\t\t";
        copy(path.begin(), path.end(), ostream_iterator<T>(cout, "\n\t\t\t"));
    }

    void dijkstraShortestPath(T, map<T, float> &distance, map<T, T> &prev);
    void makedotfile();
    vector<string> split(const string &s, char delim)
    {
        vector<string> result;
        stringstream ss(s);
        string item;

        while (getline(ss, item, delim))
        {
            result.push_back(item);
        }

        return result;
    }

    bool check(string, string);
    void calcPrice(string, string);
};

template <typename T>
bool Graph<T>::check(string src, string dest)
{
    int f = 0;
    list<string>::iterator it;
    for (it = path.begin(); it != path.end(); ++it)
    {
        if (*it == src)
        {
            f++;
        }
        else if (*it == dest)
        {
            f++;
        }
    }
    if (f == 2)
        return true;
    else
        return false;
}

template <typename T>
void Graph<T>::calcPrice(string srstn, string dstn)
{
    string line;
    int i = 0, j = 0, k = 0, len, last = 0;
    string num = "";
    long arr[235][235];
    ifstream infile("fare.csv");

    if (infile.is_open())
    {
        while (getline(infile, line))
        {

            k = 0, last = 0, j = 0;
            len = line.length();
            while (k != len)
            {
                if (line[k] == ',' || k == len - 1)
                {

                    arr[i][j] = atof(num.append(line, last, k).c_str());
                    num = "";
                    j++;

                    if (k != len - 1)
                        last = k + 1;
                }
                k++;
            }
            i++;
        }
        infile.close();
    }
    else
        cout << "Unable to open file";

    string line1;
    int a, b;
    a = 0;
    b = 0;
    ifstream file("stations.txt");
    int f = 0;
    while (!file.eof())
    {
        getline(file, line1);
        vector<string> v1 = split(line1, '\t');
        if (v1[1] == srstn)
        {
            f++;
            std::istringstream(v1[0]) >> a;
        }
        if (v1[1] == dstn)
        {
            f++;
            std::istringstream(v1[0]) >> b;
        }
        if (f == 2)
            break;
    }
    cout << endl
         << "\t\t\t";
    cout << "--> Fare is: Rs." << arr[a - 1][b - 1] << endl;
}

template <typename T>
void Graph<T>::makedotfile()
{
    string a, b, clr;
    string labl;
    int f;
    f = 0;
    clr = "red";
    char filename[] = "finalmap.dot";
    string delimiter = ",";
    ofstream fout(filename);
    fout << "graph G {" << endl;
    fout << "node [shape=rect,dpi=600] margin=0.75" << endl;
    fout << "\n//" << clr << endl;
    string x;
    ifstream file("data.txt");
    while (!file.eof())
    {
        getline(file, x);
        vector<string> v = split(x, ',');
        a = v[0];
        b = v[1];
        labl = v[2];
        if (f == 1)
        {
            fout << "\n//" << clr << endl;
            f = 0;
        }

        if (check(a, b) == true)
            fout << "\"" << a << "\"" << " -- " << "\"" << b << "\"" << " " << "[label=\"" << labl << "\",color=" << clr << " " << ",penwidth=\"8\"];" << endl;
        else
            fout << "\"" << a << "\"" << " -- " << "\"" << b << "\"" << " " << "[label=\"" << labl << "\",color=" << clr << " " << ",penwidth=\"2\"];" << endl;
        if (a == "Seelampur" && f == 0)
        {
            f = 1;
            clr = "blue";
        }
        else if (a == "Golf Course" && f == 0)
        {
            f = 1;
            clr = "green";
        }
        else if (a == "Sant Guru Ram Singh Marg" && f == 0)
        {
            f = 1;
            clr = "violet";
        }
        else if (a == "JL Nehru Stadium" && f == 0)
        {
            f = 1;
            clr = "yellow";
        }
    }
    fout << "}";
}

template <typename T>
void Graph<T>::dijkstraShortestPath(T src, map<T, float> &distance, map<T, T> &prev)
{

    set<pair<float, T>> s;
    prev.clear();
    for (auto j : adjList)
    {
        distance[j.first] = numeric_limits<float>::infinity();
        prev[j.first] = "";
    }

    distance[src] = 0;
    s.insert(make_pair(0, src));

    while (!s.empty())
    {

        auto p = *(s.begin());
        T node = p.second;

        float nodeDist = p.first;
        s.erase(s.begin());
        for (auto childPair : adjList[node])
        {
            T dest = childPair.first;
            float weight = childPair.second;
            float distance_through_node = nodeDist + childPair.second;

            if (distance_through_node < distance[childPair.first])
            {

                auto f = s.find(make_pair(distance[dest], dest));
                if (f != s.end())
                {
                    s.erase(f);
                }
                distance[dest] = distance_through_node;
                prev[dest] = node;
                s.insert(make_pair(distance[dest], dest));
            }
        }
    }
}

string stationNameInLowercase(string station)
{
    string formattedStation;
    for (char ch : station)
    {
        if (!isspace(static_cast<unsigned char>(ch)))
        {
            formattedStation += std::tolower(static_cast<unsigned char>(ch));
        }
    }
    return formattedStation;
}

string getStationNameFromUserInput(string userInput)
{
    unordered_map<string, string> stationMap = {
        {"rithala", "Rithala"},
        {"netajisubhashplace", "Netaji Subhash Place"},
        {"keshavpuram", "Keshav Puram"},
        {"kanhaiyanagar", "Kanhaiya Nagar"},
        {"inderlok", "Inderlok"},
        {"shastrinagar", "Shastri Nagar"},
        {"pratapnagar", "Pratap Nagar"},
        {"pulbangash", "Pulbangash"},
        {"tishazari", "Tis Hazari"},
        {"kashmeregate", "Kashmere Gate"},
        {"shastripark", "Shastri Park"},
        {"seelampur", "Seelampur"},
        {"welcome", "Welcome"},
        {"rajourigarden", "Rajouri Garden"},
        {"rameshnagar", "Ramesh Nagar"},
        {"motinagar", "Moti Nagar"},
        {"kirtinagar", "Kirti Nagar"},
        {"shadipur", "Shadipur"},
        {"patelnagar", "Patel Nagar"},
        {"rajenderplace", "Rajender Place"},
        {"karolbagh", "Karol Bagh"},
        {"rajivchowk", "Rajiv Chowk"},
        {"barakhambaroad", "Barakhamba Road"},
        {"mandihouse", "Mandi House"},
        {"pragatimaidan", "Pragati Maidan"},
        {"inderprastha", "Inderprastha"},
        {"yamunabank", "Yamuna Bank"},
        {"laxminagar", "Laxmi Nagar"},
        {"nirmanvihar", "Nirman Vihar"},
        {"preetvihar", "Preet Vihar"},
        {"karkarduma", "Karkar Duma"},
        {"anandvihar", "Anand Vihar"},
        {"kaushambi", "Kaushambi"},
        {"vaishali", "Vaishali"},
        {"akshardham", "Akshardham"},
        {"mayurviharphase1", "Mayur Vihar Phase-1"},
        {"mayurviharextension", "Mayur Vihar Extension"},
        {"newashoknagar", "New Ashok Nagar"},
        {"noidasector15", "Noida Sector-15"},
        {"noidasector16", "Noida Sector-16"},
        {"noidasector18", "Noida Sector-18"},
        {"botanicalgarden", "Botanical Garden"},
        {"golfcourse", "Golf Course"},
        {"noidacitycenter", "Noida City Center"},
        {"madipur", "Madipur"},
        {"shivajipark", "Shivaji Park"},
        {"punjabibagh", "Punjabi Bagh"},
        {"ashokpark", "Ashok Park"},
        {"santgururamsinghmarg", "Sant Guru Ram Singh Marg"},
        {"lalqila", "Lal Qila"},
        {"jamamasjid", "Jama Masjid"},
        {"delhigate", "Delhi Gate"},
        {"ito", "ITO"},
        {"janptah", "Janptah"},
        {"centralsecretariat", "Central Secretariat"},
        {"khanmarket", "Khan Market"},
        {"jlnehrustadium", "JL Nehru Stadium"},
        {"jangpura", "Jangpura"},
        {"vishwavidyalaya", "Vishwavidyalaya"},
        {"vidhansabha", "Vidhan Sabha"},
        {"civillines", "Civil Lines"},
        {"chandnichowk", "Chandni Chowk"},
        {"chawribazar", "Chawri Bazar"},
        {"newdelhi", "New Delhi"},
        {"patelchowk", "Patel Chowk"},
        {"udyogbhawan", "Udyog Bhawan"},
        {"lokkalyanmarg", "Lok Kalyan Marg"},
        {"jorbagh", "Jor Bagh"},
        {"samaypurbadli", "Samaypur Badli"},
        {"rohinisector18", "Rohini Sector - 18"},
        {"haiderpurbadlimor", "Haiderpur Badli Mor"},
        {"jahangirpuri", "Jahangirpuri"},
        {"adarshnagar", "Adarsh Nagar"},
        {"azadpur", "Azadpur"},
        {"modeltown", "Model Town"},
        {"gtbnagar", "GTB Nagar"},
        {"greenpark", "Green Park"},
        {"hauzkhas", "Hauz Khas"},
        {"malviyanagar", "Malviya Nagar"},
        {"saket", "Saket"},
        {"qutabminar", "Qutab Minar"},
        {"chhattarpur", "Chhattarpur"},
        {"sultanpur", "Sultanpur"},
        {"ghitorni", "Ghitorni"},
        {"arjangarh", "Arjan Garh"},
        {"gurudronacharya", "Guru Dronacharya"},
        {"sikandarpur", "Sikandarpur"},
        {"mgroad", "MG Road"},
        {"iffcochowk", "IFFCO Chowk"},
        {"hudacitycentre", "Huda City Centre"},
        {"aiims", "AIIMS"},
        {"ina", "INA"}

    };

    unordered_map<string, string>::iterator it = stationMap.find(userInput);

    if (it == stationMap.end())
        return "-1";
    else
        return it->second;
}

int main()
{
    system("echo  \"\e[96m\"");
    cout << "\t\t" << " __  __      _               __  __       _        _       " << endl;
    cout << "\t\t" << "|  \\/  | ___| |_ _ __ ___   |  \\/  | __ _| |_ _ __(_)_  __ " << endl;
    cout << "\t\t" << "| |\\/| |/ _ \\ __| '__/ _ \\  | |\\/| |/ _` | __| '__| \\ \\/ / " << endl;
    cout << "\t\t" << "| |  | |  __/ |_| | | (_) | | |  | | (_| | |_| |  | |>  <  " << endl;
    cout << "\t\t" << "|_|  |_|\\___|\\__|_|  \\___/  |_|  |_|\\__,_|\\__|_|  |_/_/\\_\\ " << endl;
    string source, destination;
    Graph<string> Metro;
    // RED-LINE
    Metro.addEdgeBetween("Rithala", "Netaji Subhash Place", 5.2);
    Metro.addEdgeBetween("Netaji Subhash Place", "Keshav Puram", 1.2);
    Metro.addEdgeBetween("Keshav Puram", "Kanhaiya Nagar", 0.8);
    Metro.addEdgeBetween("Kanhaiya Nagar", "Inderlok", 1.2);
    Metro.addEdgeBetween("Inderlok", "Shastri Nagar", 1.2);
    Metro.addEdgeBetween("Shastri Nagar", "Pratap Nagar", 1.7);
    Metro.addEdgeBetween("Pratap Nagar", "Pulbangash", 0.8);
    Metro.addEdgeBetween("Pulbangash", "Tis Hazari", 0.9);
    Metro.addEdgeBetween("Tis Hazari", "Kashmere Gate", 1.1);
    Metro.addEdgeBetween("Kashmere Gate", "Shastri Park", 2.2);
    Metro.addEdgeBetween("Shastri Park", "Seelampur", 1.6);
    Metro.addEdgeBetween("Seelampur", "Welcome", 1.1);
    // BLUE-LINE
    Metro.addEdgeBetween("Rajouri Garden", "Ramesh Nagar", 1);
    Metro.addEdgeBetween("Ramesh Nagar", "Moti Nagar", 1.2);
    Metro.addEdgeBetween("Moti Nagar", "Kirti Nagar", 1);
    Metro.addEdgeBetween("Kirti Nagar", "Shadipur", 0.7);
    Metro.addEdgeBetween("Shadipur", "Patel Nagar", 1.3);
    Metro.addEdgeBetween("Patel Nagar", "Rajender Place", 0.9);
    Metro.addEdgeBetween("Rajender Place", "Karol Bagh", 1);
    Metro.addEdgeBetween("Karol Bagh", "Rajiv Chowk", 3.4);
    Metro.addEdgeBetween("Rajiv Chowk", "Barakhamba Road", 0.7);
    Metro.addEdgeBetween("Barakhamba Road", "Mandi House", 1);
    Metro.addEdgeBetween("Mandi House", "Pragati Maiden", 0.8);
    Metro.addEdgeBetween("Pragati Maiden", "Inderprastha", 0.8);
    Metro.addEdgeBetween("Inderprastha", "Yamuna Bank", 1.8);
    Metro.addEdgeBetween("Yamuna Bank", "Laxmi Nagar", 1.3);
    Metro.addEdgeBetween("Laxmi Nagar", "Nirman Vihar", 1.1);
    Metro.addEdgeBetween("Nirman Vihar", "Preet Vihar", 1.0);
    Metro.addEdgeBetween("Preet Vihar", "Karkar Duma", 1.2);
    Metro.addEdgeBetween("Karkar Duma", "Anand Vihar", 1.1);
    Metro.addEdgeBetween("Anand Vihar", "Kaushambi", 0.8);
    Metro.addEdgeBetween("Kaushambi", "Vaishali", 1.6);
    Metro.addEdgeBetween("Yamuna Bank", "Akshardham", 1.3);
    Metro.addEdgeBetween("Akshardham", "Mayur Vihar Phase-1", 1.8);
    Metro.addEdgeBetween("Mayur Vihar Phase-1", "Mayur Vihar Extention", 1.2);
    Metro.addEdgeBetween("Mayur Vihar Extention", "New Ashok Nagar", 0.9);
    Metro.addEdgeBetween("New Ashok Nagar", "Noida Sector-15", 1.0);
    Metro.addEdgeBetween("Noida Sector-15", "Noida Sector-16", 1.1);
    Metro.addEdgeBetween("Noida Sector-16", "Noida Sector-18", 1.1);
    Metro.addEdgeBetween("Noida Sector-18", "Botanical Garden", 1.1);
    Metro.addEdgeBetween("Botanical Garden", "Golf Course", 1.2);
    Metro.addEdgeBetween("Golf Course", "Noida City Center", 1.3);
    // GREEN-LINE
    Metro.addEdgeBetween("Madipur", "Shivaji Park", 1.1);
    Metro.addEdgeBetween("Shivaji Park", "Punjabi Bagh", 1.6);
    Metro.addEdgeBetween("Punjabi Bagh", "Ashok Park", 0.9);
    Metro.addEdgeBetween("Ashok Park", "Inderlok", 1.4);
    Metro.addEdgeBetween("Ashok Park", "Sant Guru Ram Singh Marg", 1.1);
    Metro.addEdgeBetween("Sant Guru Ram Singh Marg", "Kirti Nagar", 1);
    Metro.addEdgeBetween("Kashmere Gate", "Lal Qila", 1.5);
    Metro.addEdgeBetween("Lal Qila", "Jama Masjid", 0.8);
    Metro.addEdgeBetween("Jama Masjid", "Delhi Gate", 1.4);
    Metro.addEdgeBetween("Delhi Gate", "ITO", 1.3);
    Metro.addEdgeBetween("ITO", "Mandi House", 0.8);
    Metro.addEdgeBetween("Mandi House", "Janptah", 1.4);
    Metro.addEdgeBetween("Janptah", "Central Secretariat", 1.3);
    Metro.addEdgeBetween("Central Secretariat", "Khan Market", 2.1);
    Metro.addEdgeBetween("Khan Market", "JL Nehru Stadium", 1.4);
    Metro.addEdgeBetween("JL Nehru Stadium", "Jangpura", 0.9);
    // YELLOW-LINE
    Metro.addEdgeBetween("Vishwavidyalaya", "Vidhan Sabha", 1);
    Metro.addEdgeBetween("Vidhan Sabha", "Civil Lines", 1.3);
    Metro.addEdgeBetween("Civil Lines", "Kashmere Gate", 1.1);
    Metro.addEdgeBetween("Kashmere Gate", "Chandni Chowk", 1.1);
    Metro.addEdgeBetween("Chandni Chowk", "Chawri Bazar", 1);
    Metro.addEdgeBetween("Chawri Bazar", "New Delhi", 0.8);
    Metro.addEdgeBetween("New Delhi", "Rajiv Chowk", 1.1);
    Metro.addEdgeBetween("Rajiv Chowk", "Patel Chowk", 1.3);
    Metro.addEdgeBetween("Patel Chowk", "Central Secretariat", 0.9);
    Metro.addEdgeBetween("Central Secretariat", "Udyog Bhawan", 0.3);
    Metro.addEdgeBetween("Udyog Bhawan", "Lok Kalyan Marg", 1.6);
    Metro.addEdgeBetween("Lok Kalyan Marg", "Jor Bagh", 1.2);
    Metro.addEdgeBetween("Samaypur Badli", "Rohini Sector - 18", 0.8);
    Metro.addEdgeBetween("Rohini Sector - 18", "Haiderpur Badli Mor", 1.3);
    Metro.addEdgeBetween("Haiderpur Badli Mor", "Jahangirpuri", 1.3);
    Metro.addEdgeBetween("Jahangirpuri", "Adarsh Nagar", 1.3);
    Metro.addEdgeBetween("Adarsh Nagar", "Azadpur", 1.5);
    Metro.addEdgeBetween("Azadpur", "Model Town", 1.4);
    Metro.addEdgeBetween("Model Town", "GTB Nagar", 1.4);
    Metro.addEdgeBetween("GTB Nagar", "Vishwavidyalaya", 0.8);
    Metro.addEdgeBetween("Jor Bagh", "INA", 1.3);
    Metro.addEdgeBetween("INA", "AIIMS", 0.8);
    Metro.addEdgeBetween("AIIMS", "Green Park", 1.0);
    Metro.addEdgeBetween("Green Park", "Hauz Khas", 1.8);
    Metro.addEdgeBetween("Hauz Khas", "Malviya Nagar", 1.7);
    Metro.addEdgeBetween("Malviya Nagar", "Saket", 0.9);
    Metro.addEdgeBetween("Saket", "Qutab Minar", 1.7);
    Metro.addEdgeBetween("Qutab Minar", "Chhattarpur", 1.3);
    Metro.addEdgeBetween("Chhattarpur", "Sultanpur", 1.6);
    Metro.addEdgeBetween("Sultanpur", "Ghitorni", 1.3);
    Metro.addEdgeBetween("Ghitorni", "Arjan Garh", 2.7);
    Metro.addEdgeBetween("Arjan Garh", "Guru Dronacharya", 2.3);
    Metro.addEdgeBetween("Guru Dronacharya", "Sikandarpur", 1.0);
    Metro.addEdgeBetween("Sikandarpur", "MG Road", 1.2);
    Metro.addEdgeBetween("MG Road", "IFFCO Chowk", 1.1);
    Metro.addEdgeBetween("IFFCO Chowk", "Huda City Centre", 1.5);
    map<string, float> distance;
    map<string, string> prev;
    string sourceStn, destStn;
    cout << endl
         << endl
         << endl;

    system("echo  \"\e[32m\"");
    cout << "\t\t";
    cout << "Enter source station :";
    getline(cin, sourceStn);
    cout << endl;

    while (getStationNameFromUserInput(stationNameInLowercase(sourceStn)) == "-1")
    {
        cout << "\t\t";
        cout << "Invalid Station Name! Enter a valid station name :";
        getline(cin, sourceStn);
        cout << endl;
    }

    sourceStn = getStationNameFromUserInput(stationNameInLowercase(sourceStn));

    system("echo  \"\e[33m\"");
    cout << "\t\t";
    cout << "Enter destination station in Title Case: ";
    getline(cin, destStn);

    while (getStationNameFromUserInput(stationNameInLowercase(destStn)) == "-1")
    {
        cout << "\t\t";
        cout << "Invalid Station Name! Enter a valid station name :";
        getline(cin, destStn);
        cout << endl;
    }

    destStn = getStationNameFromUserInput(stationNameInLowercase(destStn));

    Metro.dijkstraShortestPath(sourceStn, distance, prev);

    system("echo  \"\e[34m\"");
    cout << endl
         << "\t\t";
    cout << "Distance from " << sourceStn << " to " << destStn << " - " << distance[destStn] << " Kms" << endl;
    cout << endl
         << "\t\tPath: " << endl;
    Metro.dijkstraGetShortestPathTo(destStn, prev);
    Metro.makedotfile();
    Metro.calcPrice(sourceStn, destStn);
    cout << endl;
    return 0;
}
