#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>

using namespace std;

vector<string> getStrVector(string file_path)
{
    ifstream file(file_path);
    vector<string> output;
    stringstream ss;

    ss << file.rdbuf();

    string file_content = ss.str();

    output.emplace_back("");

    for (char &ch : file_content)
    {
        if (ch == ' ' || ch == '\n')
        {
            output.emplace_back("");
            continue;
        }

        output[output.size() - 1] += ch;
    }

    return output;
}

void writeVector(string file_path, vector<string> lines)
{
    ofstream file(file_path);

    for (string line : lines)
    {
        line = line+"\n";
        file.write(line.c_str(), line.size());
    }
    
}

int fitWords(vector<string> &words, int maxWidth, int left, string &line)
{
    int r = left, width = 0;
    while (r < words.size())
    {
        int sz = words[r].size();
        if (width + sz > maxWidth)
        {
            r--;
            break;
        }
        width += sz + 1;
        r++;
    }

    int spaces = maxWidth - width + r - left + 1;

    string str = "";

    if (r >= words.size())
    {
        int anti_space = 0;
        for (int i = left; i < words.size(); ++i)
        {
            string sp = " ";
            if (i == words.size() - 1)
                sp = "";
            str += words[i] + sp;
            anti_space += words[i].size() + sp.size();
        }

        if (maxWidth > anti_space)
            str += string(maxWidth - anti_space, ' ');
        line = str;
        return r;
    }

    if (r - left <= 0)
    {
        str += words[left];
        str += string(maxWidth - words[left].size(), ' ');
        line = str;
        return r;
    }

    int per_space = spaces / (r - left), extra = spaces % (r - left);

    for (int i = left; i < r; ++i)
    {
        str += words[i] + string(per_space, ' ');
        if (extra > 0)
        {
            str += " ";
            extra--;
        }
    }

    str += words[r];
    line = str;
    return r;
}

vector<string> fullJustify(vector<string> &words, int maxWidth)
{
    vector<string> output{};

    int left = 0, right = 0;
    string line;

    while (right < words.size())
    {
        right = fitWords(words, maxWidth, left, line);
        left = right + 1;
        output.emplace_back(line);
    }

    return output;
}

int main(int argc, char *argv[])
{
    string ifile = "", ofile = "";
    int max_width = 100;

    for (int i = 0; i < argc; ++i)
    {
        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc)
            ifile = argv[++i];

        if (strcmp(argv[i], "-o") == 0 && i + 1 < argc)
            ofile = argv[++i];

        if (strcmp(argv[i], "-w") == 0 && i + 1 < argc)
            max_width = stoi(argv[++i]);
    }

    bool isError = false;

    if (ifile.size() == 0)
    {
        cout << "MAIN::ERROR:: Input file expected!" << endl;
        isError = true;
    }
    if (ofile.size() == 0)
    {
        cout << "MAIN::ERROR:: Output file expected!" << endl;
        isError = true;
    }
    if (max_width == 100)
    {
        cout << "MAIN::WARNING:: default max width of line is 100 chars" << endl;
    }

    if (isError) return EXIT_FAILURE;

    ifstream file(ifile);
    vector<string> words = getStrVector(ifile);

    vector<string> output = fullJustify(words, max_width);

    writeVector(ofile, output);

    return 0;
}