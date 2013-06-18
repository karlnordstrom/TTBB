#include <Utils.hh>

using namespace std;

bool areSimilar(float reference, float estimate, float absolute, float relative) {
    return (estimate > (reference - (absolute + relative * abs(reference)))) && (estimate < (reference + (absolute + relative * abs(reference))));
}


AverageHisto::AverageHisto(TH1D* histo)
    : _min((float)histo->GetBinLowEdge(1)), _max((float)histo->GetBinLowEdge(histo->GetNbinsX() + 1)), _bin((float)(_max-_min)/(float)histo->GetNbinsX())
{
    if(histo->GetNbinsX() < 1)throw Exception("AverageHisto constructed with invalid histogram.");
    _array.clear();
    _entries.clear();
    _averages.clear();
    _histogram = histo;
    _array.resize(histo->GetNbinsX());
    _entries.resize(histo->GetNbinsX());
    _averages.resize(histo->GetNbinsX());
}

AverageHisto::AverageHisto(int array, float min, float max)
    : _min(min), _max(max), _bin((float)(max-min)/array)
{
    if(array < 1 || max < min)throw Exception("AverageHisto constructed with nonsensical values.");
    _array.clear();
    _entries.clear();
    _averages.clear();
    _histogram = NULL;
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

void AverageHisto::fillHisto() {
    if(_histogram == NULL)throw Exception("Need to construct with pointer to histogram to fill without specifying histogram.");
    setAverage();
    for(int i = 0; i < (int)_averages.size(); i++) {
        _histogram->Fill(_min + round(i*_bin - 0.49), _averages[i]);
    }
}

