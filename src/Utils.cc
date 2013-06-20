#include <Utils.hh>

using namespace std;

namespace Analysis {

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

void AverageHisto::fillHisto() {
    if(_histogram == NULL)throw Exception("Need to construct with pointer to histogram to fill without specifying histogram.");
    TH1* histo = (TH1*)_histogram;
    setAverage();
    for(int i = 0; i < (int)_averages.size(); i++) {
        histo->Fill(_min + round(i*_bin - 0.49), _averages[i]);
    }
}

} // end namespace
