#ifndef Utils_HH
#define Utils_HH

#include <Math/Vector4D.h>
#include <TLorentzVector.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TVector2.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TChain.h>
#include <TROOT.h>
#include <TStyle.h>

#include <cmath>
#include <vector>
#include <iostream>
#include <boost/foreach.hpp>
#include <exception>
#include <stdexcept>
#include <boost/any.hpp>
#include <algorithm>

#define foreach BOOST_FOREACH //just to clean up the loops a bit

using namespace std;

namespace Analysis {

/// Class for throwing exceptions
class Exception : public std::runtime_error {

public:
    /// Constructor with error description string
    Exception(const std::string& what) : std::runtime_error(what) {}

};

/// Helper class for creating histos of average values:
/// much nicer than dividing two histos!
class AverageHisto {

public:

    AverageHisto(int array, float min, float max);


    /// Templated constructor to allow the class to be constructed
    /// with any kind of histogram
    template <class H>
    AverageHisto(H* histo)
      : _min((float)histo->GetBinLowEdge(1)), _max((float)histo->GetBinLowEdge(histo->GetNbinsX() + 1)), _bin((float)(_max-_min)/(float)histo->GetNbinsX())
    {
      if(histo->GetNbinsX() < 1)throw Exception("AverageHisto constructed with invalid histogram.");
      _array.clear();
      _entries.clear();
      _averages.clear();
      _histogram = (boost::any*)histo;
      _array.resize(histo->GetNbinsX());
      _entries.resize(histo->GetNbinsX());
      _averages.resize(histo->GetNbinsX());
    }

    ~AverageHisto();
    void add(float x, float y);
    void setAverage();
    vector<float> getAverage();

    /// Templated filler for when class manually constructed
    template <class H>
    void fillHisto(H* histo) {
      setAverage();
      for(int i = 0; i < (int)_averages.size(); i++) {
        histo->Fill(_min + round(i*_bin - 0.49), _averages[i]);
      }
    }

    void fillHisto();

private:

    float _min;
    float _max;
    float _bin;
    vector<float> _array;
    vector<int> _entries;
    vector<float> _averages;
    boost::any* _histogram; // because boost::any is AWESOME

};

} // end namespace

#endif
