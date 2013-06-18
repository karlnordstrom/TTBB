#include "library.h"

using namespace std;

bool areSimilar(float reference, float estimate, float absolute, float relative) {
    return (estimate > (reference - (absolute + relative * abs(reference)))) && (estimate < (reference + (absolute + relative * abs(reference))));
}

vector<TLorentzVector> makeVectors(vector<float> pt, vector<float> eta, vector<float> phi, vector<float> E) {
    assert(pt.size() == eta.size() && eta.size() == phi.size() && phi.size() == E.size());//make sure input makes sense
    vector<TLorentzVector> vectors;
    TLorentzVector vector;
    for(unsigned int it = 0; it < pt.size(); it++) {
        vector.SetPtEtaPhiE(pt[it]/1000., eta[it], phi[it], E[it]/1000.); // convert to GeV
        vectors.push_back(vector);
    }
    return vectors;
}

AverageHisto::AverageHisto(int array, float min, float max)
    : _min(min), _max(max), _bin((float)(max-min)/array)
{
    _array.resize(array);
    _entries.resize(array);
    _averages.resize(array);
}

AverageHisto::~AverageHisto() {}

void AverageHisto::add(float x, float y) {
    if(x > _max || x < _min)return;
    int index = round((x - _min)/_bin - 0.49);
    _array[index] += y;
    _entries[index]++;
}

void AverageHisto::setAverage() {
    for(int i = 0; i < (int)_averages.size(); i++) {
        if (_entries[i] != 0)_averages[i] = (float)_array[i] / _entries[i];
    }
}

vector<float> AverageHisto::getAverage() {
    setAverage();
    return _averages;
}

void AverageHisto::fillHisto(TH1D* histo) {
    setAverage();
    for(int i = 0; i < (int)_averages.size(); i++) {
        histo->Fill(_min + round(i*_bin - 0.49), _averages[i]);
    }
}
