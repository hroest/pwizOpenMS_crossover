//
// Hello World program that OpenMS and proteowizard. 
// It computes the TIC of a mzML file.
//
// Author: Hannes Roest
//

//  - Performance tested on 2 files of different size:
//
// File 1
// pwiz 1.8 s / 109 MB
// OpenMS 7-8 s / 104 MB
//
// File 2
// pwiz  20s /  146 MB
// OpenMS 57 s / 1 GB 
//

#include "pwiz/data/msdata/MSDataFile.hpp"
#include <iostream>
#include <numeric>

#include <OpenMS/FORMAT/MzMLFile.h>
#include <OpenMS/CONCEPT/ProgressLogger.h>
#include <OpenMS/ANALYSIS/OPENSWATH/CachedmzML.h>

using namespace std;
using namespace pwiz::msdata;
using namespace OpenMS;

void doSomething(MSData& msd)
{
    // manipulate your MSData object here
    SpectrumListPtr sl = msd.run.spectrumListPtr;
     
    double TIC = 0;

    if (sl.get())
    {
        cout << "# of spectra: " << sl->size() << endl;
        for(int i = 0; i< sl->size(); i++)
        {
          SpectrumPtr sptr = sl->spectrum(i, true);
          if(sptr->hasBinaryData())
          {
            BinaryDataArrayPtr intensities = sptr->getIntensityArray();
            std::vector<double> data = intensities->data;
            TIC += std::accumulate(data.begin(), data.end(), 0.0);
          }
          else
          {
            std::cerr << "Error, no binary data" << std::endl;
          }
        }
    }
    cout << "total TIC  " << TIC << endl;

}

void doSomething(MSExperiment<>& msd)
{
    // manipulate your MSData object here
    double TIC = 0;

    cout << "# of spectra: " << msd.size() << endl;
    for(int i = 0; i< msd.size(); i++)
    {
      MSSpectrum<> sptr = msd[i];
      for(MSSpectrum<>::iterator it = sptr.begin(); it != sptr.end(); it++)
      {
        TIC += it->getIntensity();
      }

    }
    cout << "total TIC  " << TIC << endl;

}

void helloPwiz(const string& filename)
{
    cout << "Hello, pwiz!\n";

    MSDataFile msd(filename);

    doSomething(msd);
}

void helloOpenMS(const string& filename)
{
    cout << "Hello, OpenMS!\n";

    MSExperiment<Peak1D> exp;
    MzMLFile().load(filename,exp);

    doSomething(exp);
}

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
            throw runtime_error("Usage: hello_pwiz filename"); 
        
        helloOpenMS(argv[1]); 
        helloPwiz(argv[1]); 

        return 0;
    }
    catch (exception& e)
    {
        cerr << e.what() << endl;
    }
    catch (...)
    {
        cerr << "Caught unknown exception.\n";
    }

    return 1;
}

