/*
 main.cpp
 File Creator and Sorter
 
 First personal c++ project
 
 This program takes an input from the user in the form of /Users/.../.../.../... , ... being a folder location.
 It then goes through the files in the folder, finds any extensions, creates folders for those extensions, and moves the files there.
 There is a chance for a few files to be located in the folder that don't have a file type. For those files I plan to implement a
 miscallaneos folder and move them there. Do not use this program unless you confirmed you want to sort everything in the folder.
 The reason being this will not leave any file alone and can mess up any set ups.
 

 Created by yared yohannes on 12/15/21.
*/

//libraries
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <set>

//namespaces
using namespace std;
namespace fs = filesystem;

//return the filetype for each file in vector, if no files returns it as miscellanous.
string getFileType(const fs::path &file) {
    string ext = file.extension();
    if (ext.size() > 1)
        return ext.substr(1);
    return "miscellaneous";
}

//creates folders for each filetype in the vector.
void makeFolders(const fs::path &folder, const set<string> &fileTypes) {
    for (const auto &fileType : fileTypes) {
        if (fileType != "DS_Store") {
            fs::path newFolder = folder / fileType;
            error_code ec;
            fs::create_directories(newFolder, ec);
            if (ec) {
                cerr << newFolder << " Could not be created." << endl;
            }
        }
    }
}

//moves the files to their correspoding folders, including none extensions to miscellaneous
void moveFiles(const vector<fs::path> &files, const fs::path &folder) {
    for (const auto &file : files) {
        error_code ec;
        fs::rename(file, folder / getFileType(file) / file.filename(), ec);
        if (ec) {
            cerr << file << " Could not be moved." << endl;
        }
    }
}

int main()
{
    
    cout << "What is the path of the folder: ";

    string dirPath;
    getline(cin, dirPath);

    error_code ec;
    fs::directory_iterator dir(dirPath, ec);
                    
    if (ec)
    {
        cerr << "ERROR: Invalid directory path." << endl;
    }
    else
    {
        vector<fs::path> files;
        set<string> fileTypes;

        for (const auto &entry : dir) {
            fs::path file = entry.path();
            files.push_back(file);
            fileTypes.insert(getFileType(file));
        }

        makeFolders(dirPath, fileTypes);
        moveFiles(files, dirPath);
        
        cout << "The files have been sorted into their proper folders. Thanks for using.";
    }

    return 0;
}
