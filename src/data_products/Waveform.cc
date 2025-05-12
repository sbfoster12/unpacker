#include "Waveform.hh"
#include <numeric>

using namespace dataProducts;

Waveform::Waveform()
    : DataProduct(),
    crateNum(0),
    amcNum(0), 
    channelTag(0),
    eventNum(0),
    waveformIndex(0),
    length(0),
    pedestalLevel(0),
    pedestalStdev(0),
    firstSampleNum(0),
    clockCounter(0),
    digitizationShift(0),
    preTriggerLength(0),
    trace({}),
    detectorType(""),
    detectorNum(0)
{}

Waveform::Waveform(int crateNumArg
    ,int amcNumArg
    ,int channelTagArg
    ,int eventNumArg
    ,int waveformIndexArg
    ,unsigned int lengthArg
    ,double pedestalLevelArg
    ,double pedestalStdevArg
    ,int firstSampleNumArg
    ,unsigned long clockCounterArg
    ,int preTriggerLengthArg
    ,int runNumArg
    ,int subRunNumArg
    ,std::vector<short> traceArg
    ) : DataProduct()
    ,crateNum(crateNumArg)
    ,amcNum(amcNumArg)
    ,channelTag(channelTagArg)
    ,eventNum(eventNumArg)
    ,waveformIndex(waveformIndexArg)
    ,length(lengthArg)
    ,pedestalLevel(pedestalLevelArg)
    ,pedestalStdev(pedestalStdevArg)
    ,firstSampleNum(firstSampleNumArg)
    ,clockCounter(clockCounterArg)
    ,preTriggerLength(preTriggerLengthArg)
    ,digitizationShift(0)
    ,runNum(runNumArg)
    ,subRunNum(subRunNumArg)
    ,x(0.0)
    ,y(0.0)
    ,is_clipping(false)
    ,trace(traceArg)
    ,detectorType("")
    ,detectorNum(0)
    ,digitizationFrequency(1.0)
{
}

Waveform::Waveform(Waveform* w) : DataProduct()
    ,runNum(w->runNum)
    ,subRunNum(w->subRunNum)
    ,crateNum(w->crateNum)
    ,amcNum(w->amcNum)
    ,channelTag(w->channelTag)
    ,eventNum(w->eventNum)
    ,waveformIndex(w->waveformIndex)
    ,length(w->length)
    ,pedestalLevel(w->pedestalLevel)
    ,pedestalStdev(w->pedestalStdev)
    ,firstSampleNum(w->firstSampleNum)
    ,clockCounter(w->clockCounter)
    ,preTriggerLength(w->preTriggerLength)
    ,digitizationShift(w->digitizationShift)
    ,trace(w->trace)
    ,raw(w)
    ,x(w->x)
    ,y(w->y)
    ,is_clipping(w->is_clipping)
    ,detectorType(w->detectorType)
    ,detectorNum(w->detectorNum)
    ,digitizationFrequency(w->digitizationFrequency)
{
}

Waveform::~Waveform() {}

void Waveform::Show() const {
    std::ostringstream oss;
    oss << std::endl;
    oss << "Waveform:" << std::endl;
    oss << "    runNum: " << runNum << std::endl;
    oss << "    subRunNum: " << subRunNum << std::endl;
    oss << "    crateNum: " << crateNum << std::endl;
    oss << "    amcSlot: " << amcNum << std::endl;
    oss << "    channelTag: " << channelTag << std::endl;
    oss << "    detectorType: " << detectorType << std::endl;
    oss << "    detectorNum: " << detectorNum << std::endl;
    oss << "    eventNum: " << eventNum << std::endl;
    oss << "    waveformIndex: " << waveformIndex << std::endl;
    oss << "    pedestalLevel: " << pedestalLevel << std::endl;
    oss << "    pedestalStdev: " << pedestalStdev << std::endl;
    oss << "    first sample number: " << firstSampleNum << std::endl;
    oss << "    clock counter: " << clockCounter << std::endl;
    oss << "    preTriggerLength: " << preTriggerLength << std::endl;
    oss << "    digitizationFrequency: " << digitizationFrequency << std::endl;
    oss << "    digitizationShift: " << digitizationShift << std::endl;
    oss << "    trace.size(): " << trace.size() << std::endl;
    oss << "        trace: ";
    for (const auto & sample : trace) {
        oss << sample << ", ";
    }
    oss << std::endl;
    std::cout << oss.str();
}
