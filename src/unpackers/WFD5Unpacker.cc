#include "WFD5Unpacker.hh"

using namespace unpackers;

WFD5Unpacker::WFD5Unpacker() 
    : PayloadUnpacker()
    , WFD5HeaderPtrCol_(std::make_shared<dataProducts::DataProductPtrCollection>())
    , ChannelHeaderPtrCol_(std::make_shared<dataProducts::DataProductPtrCollection>())
    , WaveformHeaderPtrCol_(std::make_shared<dataProducts::DataProductPtrCollection>())
    , WaveformPtrCol_(std::make_shared<dataProducts::DataProductPtrCollection>())
    , WFD5HeaderParser_(std::make_unique<parsers::WFD5HeaderParser>())
    , ChannelHeaderParser_(std::make_unique<parsers::ChannelHeaderParser>())
    , WaveformHeaderParser_(std::make_unique<parsers::WaveformHeaderParser>())
    , WaveformParser_(std::make_unique<parsers::WaveformParser>())
    , ChannelHeaderAsyncParser_(std::make_unique<parsers::ChannelHeaderAsyncParser>())
    , WaveformHeaderAsyncParser_(std::make_unique<parsers::WaveformHeaderAsyncParser>())
{
    utils::LoggerHolder::getInstance()->InfoLogger << "We are constructing the WFD5 unpacker." << std::endl;

    //Register the collections
    this->RegisterCollection("WFD5HeaderCollection",WFD5HeaderPtrCol_);
    this->RegisterCollection("ChannelHeaderCollection",ChannelHeaderPtrCol_);
    this->RegisterCollection("WaveformHeaderCollection",WaveformHeaderPtrCol_);
    this->RegisterCollection("WaveformCollection",WaveformPtrCol_);

}

WFD5Unpacker::~WFD5Unpacker() {};

int WFD5Unpacker::Unpack(const uint64_t* words, unsigned int& wordNum) {
    utils::LoggerHolder::getInstance()->InfoLogger << "\tWe are unpacking a WFD5 payload." << std::endl;

    //Store the starting word for comparison at the end
    unsigned int startWordNum = wordNum;

    /*
        First up is the AMC13+WFD5 header, which is 3 words long
    */

    auto amc_header_words = GetXWords(words,wordNum,3,"bigendian");

    //Set the words in the parser
    WFD5HeaderParser_->SetWords(amc_header_words);
    utils::LoggerHolder::getInstance()->DebugLogger << WFD5HeaderParser_->Stream().str();

    //Get the AMC slot num
    int amcSlot = WFD5HeaderParser_->AMCNo();

    //Get the data length
    int data_length = WFD5HeaderParser_->DataLength();

    //Get the number of enabled channels
    unsigned int enabled_channels = WFD5HeaderParser_->NumEnabledChannels();

    //Get the time stamp
    clockCounter_ = WFD5HeaderParser_->ClockCounter();

    //Is it synchronous or asynchronous?
    auto aSyncMode = WFD5HeaderParser_->ASyncMode();

    //Is the event empty?
    if (WFD5HeaderParser_->EmptyEvent()) {
        utils::LoggerHolder::getInstance()->InfoLogger << "\t\tThis event is empty. Not unpacking." << std::endl;
        //increment the current word and return
        wordNum+=data_length;
        return FAILURE_UNPACKING;
    }

    //Create the data product
    WFD5HeaderPtrCol_->push_back(WFD5HeaderParser_->NewDataProduct(crateNum_));

    //Clear data from parser
    WFD5HeaderParser_->Clear();

    /*
        Now we loop over the channels
    */
    for (unsigned int iChannel = 0; iChannel < enabled_channels; ++iChannel)
    {
        //std::cout << "        Proceesing channel: " << iChannel << std::endl;

        //Check which mode we are in: synchronous or asynchronous
        if (aSyncMode) { this->UnpackASyncMode(words,wordNum,amcSlot); }
        else { this->UnpackSyncMode(words,wordNum,amcSlot); }

    }

    //Trailers
    wordNum+=1;

    //Check that we have processed exactly the data_length
    if ((wordNum - startWordNum) != data_length) {
        std::cerr << "Error: didn't parse entire data length\n"
        << "Location: WFD5Unpacker::Unpack(uint64_t* words, unsigned int& wordNum)\n"
        << "Details: the data length is " << data_length << ", but the number of parse words is " << wordNum - startWordNum << std::endl;
        return FAILURE_UNPACKING;
    }
    return SUCCESS_UNPACKING;
};


void WFD5Unpacker::UnpackSyncMode(const uint64_t* words, unsigned int& wordNum, int amcSlot) {

    /*
        Parse the Channel header, which is 2 64 bit words long
    */
    auto ch_header_words = GetXWords(words,wordNum,2,"bigendian");

    //Set the words in the channel parser
    //We won't write the data product yet until we get the trailer
    ChannelHeaderParser_->SetWords(ch_header_words);
    utils::LoggerHolder::getInstance()->DebugLogger << ChannelHeaderParser_->Stream().str();

    //Get the channel tag
    uint32_t channelTag = ChannelHeaderParser_->ChannelTag();

    //Get the number of waveforms in this channel
    unsigned int waveform_count = ChannelHeaderParser_->WaveformCount();

    /*
        Now we loop over each waveform
    */
    for (unsigned int iWaveform = 0; iWaveform < waveform_count; ++iWaveform)
    {
        //std::cout << "            Proceesing waveform: " << iWaveform << std::endl;
        /*
            Parse the Waveform header, which is 2 64 bit words long
        */
        auto wf_header_words = GetXWords(words,wordNum,2,"bigendian");

        //Set the words in the parser
        WaveformHeaderParser_->SetWords(wf_header_words);
        utils::LoggerHolder::getInstance()->DebugLogger << WaveformHeaderParser_->Stream().str();

        //Get the waveform length (number of 8 bit ADC samples = 2 64 bit words)
        uint32_t waveform_length = WaveformHeaderParser_->WaveformLength();

        uint32_t preTriggerLength = WaveformHeaderParser_->PreTriggerLength();

        //Get the waveform index
        int waveformIndex = 0;

        //Create the data product
        WaveformHeaderPtrCol_->push_back(WaveformHeaderParser_->NewDataProduct(crateNum_,amcSlot));

        //Clean up
        WaveformHeaderParser_->Clear();

        /*
            Now get the ADC data , which is waveform_length*2 64-bit words
        */
        auto adc_words = GetXWords(words,wordNum,waveform_length*2,"le");

        //Set the words in the parser
        WaveformParser_->SetWords(adc_words);
        utils::LoggerHolder::getInstance()->DebugLogger << WaveformParser_->Stream().str();

        //Create the data product
        WaveformPtrCol_->push_back(std::make_unique<dataProducts::Waveform>(
            crateNum_
            ,amcSlot
            ,channelTag
            ,eventNum_
            ,waveformIndex
            ,waveform_length*8 //waveform_length is the number of 8 ADC values
            ,0. // pedestal
            ,0. // pedestal stdev
            ,0  // first sample number
            ,clockCounter_
            ,preTriggerLength*2 //preTriggerLength is in units of 2 ADC values
            ,0  // run number
            ,0  // subrun number
            ,WaveformParser_->ADCData()
        ));

        //Clean up
        WaveformParser_->Clear();

    }

    //channel trailer
    auto ch_trailer_words = GetXWords(words,wordNum,3,"bigendian");
    ChannelHeaderParser_->SetTrailer(ch_trailer_words);

    //Create the data product for the channel header now that we have the channel trailer
    ChannelHeaderPtrCol_->push_back(ChannelHeaderParser_->NewDataProduct(crateNum_,amcSlot));

    //Clean up the parser
    ChannelHeaderParser_->Clear();

}

void WFD5Unpacker::UnpackASyncMode(const uint64_t* words, unsigned int& wordNum, int amcSlot) {

    /*
        Parse the Channel header, which is 2 64 bit words long
    */
    auto ch_header_words = GetXWords(words,wordNum,2,"bigendian");

    //Set the words in the channel parser
    //We won't write the data product yet until we get the trailer
    ChannelHeaderAsyncParser_->SetWords(ch_header_words);
    utils::LoggerHolder::getInstance()->DebugLogger << ChannelHeaderAsyncParser_->Stream().str();

    //Get the channel tag
    uint32_t channelTag = ChannelHeaderAsyncParser_->ChannelTag();

    //Get the number of waveforms in this channel
    unsigned int waveform_count = ChannelHeaderAsyncParser_->WaveformCount();

    /*
        Now we loop over each waveform
    */
    for (unsigned int iWaveform = 0; iWaveform < waveform_count; ++iWaveform)
    {
        //std::cout << "            Proceesing waveform: " << iWaveform << std::endl;
        /*
            Parse the Waveform header, which is 2 64 bit words long
        */
        auto wf_header_words = GetXWords(words,wordNum,2,"bigendian");

        //Set the words in the parser
        WaveformHeaderAsyncParser_->SetWords(wf_header_words);
        utils::LoggerHolder::getInstance()->DebugLogger << WaveformHeaderAsyncParser_->Stream().str();

        //Get the waveform length (number of 8 bit ADC samples = 2 64 bit words)
        uint32_t waveform_length = WaveformHeaderAsyncParser_->WaveformLength();

        //Get the waveform index
        int waveformIndex = WaveformHeaderAsyncParser_->WaveformIndex();

        //In the async mode, each waveform has its own timestamp
        unsigned long thisClockCounter = WaveformHeaderAsyncParser_->ClockCounter();

        uint32_t preTriggerLength = WaveformHeaderAsyncParser_->PreTriggerLength();

        //Create the data product
        WaveformHeaderPtrCol_->push_back(WaveformHeaderAsyncParser_->NewDataProduct(crateNum_,amcSlot));

        //Clean up
        WaveformHeaderAsyncParser_->Clear();

        /*
            Now get the ADC data , which is waveform_length*2 64-bit words
        */
        auto adc_words = GetXWords(words,wordNum,waveform_length*2,"le");

        //Set the words in the parser
        WaveformParser_->SetWords(adc_words);
        utils::LoggerHolder::getInstance()->DebugLogger << WaveformParser_->Stream().str();

        //Create the data product
        WaveformPtrCol_->push_back(std::make_unique<dataProducts::Waveform>(
            crateNum_
            ,amcSlot
            ,channelTag
            ,eventNum_
            ,waveformIndex
            ,waveform_length*8 //waveform_length is the number of 8 ADC values
            ,0. // pedestal
            ,0. // pedestal stdev
            ,0  // first sample number
            ,thisClockCounter
            ,preTriggerLength*2 //preTriggerLength is in units of 2 ADC values
            ,0  // run number
            ,0  // subrun number
            ,WaveformParser_->ADCData()
        ));

        //Clean up
        WaveformParser_->Clear();

    }

    //channel trailer
    auto ch_trailer_words = GetXWords(words,wordNum,3,"bigendian");
    ChannelHeaderAsyncParser_->SetTrailer(ch_trailer_words);

    //Create the data product for the channel header now that we have the channel trailer
    ChannelHeaderPtrCol_->push_back(ChannelHeaderAsyncParser_->NewDataProduct(crateNum_,amcSlot));

    //Clean up the parser
    ChannelHeaderAsyncParser_->Clear();
}

