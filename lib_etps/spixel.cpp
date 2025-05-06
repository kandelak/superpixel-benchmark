// spixel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "segengine.h"
#include "functions.h"
#include "utils.h"
#include <fstream>
#include <cstdlib>

using namespace cv;
using namespace std;

void ProcessFilesBatch(SPSegmentationParameters& params, const vector<string>& files, const string& fileDir)
{
    MkDir(fileDir + "out");
    MkDir(fileDir + "seg");

    int nProcessed = 0;
    double totalTime = 0.0;

    for (const string& f : files) {
        string fileName = fileDir + f;
        Mat image = imread(fileName, cv::IMREAD_COLOR);

        if (image.rows == 0 || image.cols == 0) {
            cout << "Failed reading image '" << fileName << "'" << endl;
            continue;
        }

        cout << "Processing: " << fileName << endl;

        SPSegmentationEngine engine(params, image);

        engine.ProcessImage();

        engine.PrintPerformanceInfo();
        totalTime += engine.ProcessingTime();

        string outImage = ChangeExtension(fileDir + "out/" + f, "_sp.png");
        string outImageSeg = ChangeExtension(fileDir + "seg/" + f, ".png");

        imwrite(outImage, engine.GetSegmentedImage());
        imwrite(outImageSeg, engine.GetSegmentation());

        nProcessed++;
    }

    if (nProcessed > 1 && params.timingOutput) {
        cout << "Processed " << nProcessed << " files in " << totalTime << " sec. ";
        cout << "Average per image " << (totalTime / nProcessed) << " sec." << endl;
    }
}

void ProcessFilesBatch(SPSegmentationParameters& params, const string& dirName, const string& pattern)
{
    vector<string> files;
    string fileDir = dirName;

    FindFiles(fileDir, pattern, files, false);
    EndDir(fileDir);
    ProcessFilesBatch(params, files, fileDir);
}

void ProcessFile(SPSegmentationParameters& params, const string& file)
{
    vector<string> files;
    string fileDir = FilePath(file);
    string fileName = FileName(file);

    files.push_back(fileName);
    ProcessFilesBatch(params, files, fileDir);
}

void ProcessFiles(const string& paramFile, const string& name1, const string& name2, const string& name3)
{
    SPSegmentationParameters params = ReadParameters(paramFile);

    if (params.randomSeed > 0) {
        srand(params.randomSeed);
    }

    // Only non-stereo processing
    if (params.batchProcessing) ProcessFilesBatch(params, name1, name2);
    else ProcessFile(params, name1);
}

int main(int argc, char* argv[])
{
    if (argc == 3) {
        ProcessFiles(argv[1], argv[2], "", "");
    } else if (argc == 4) {
        ProcessFiles(argv[1], argv[2], argv[3], "");
    } else if (argc == 5) {
        ProcessFiles(argv[1], argv[2], argv[3], argv[4]);
    } else {
        cout << argc << endl;
        for (int i = 1; i < argc; i++) {
            cout << argv[i] << "  ";
        }
        cout << endl;
        cout << "Real-Time Coarse-to-fine Topologically Preserving Segmentation, CVPR 2015" << endl;
        cout << "Built on: " << __DATE__ << " " << __TIME__ << endl;
        cout << "Usage (stereo = 0 & batchProcessing = 0): spixel config_file.yml file_name" << endl;
        cout << "   or (stereo = 0 & batchProcessing = 1): spixel config_file.yml file_dir file_pattern" << endl;
    }
    return 0;
}
