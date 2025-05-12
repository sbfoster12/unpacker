#if defined(__ROOTCLING__)

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class dataProducts::DataProduct+;
#pragma link C++ class std::vector<dataProducts::DataProduct>+;

#pragma link C++ class dataProducts::WFD5Header+;
#pragma link C++ class std::vector<dataProducts::WFD5Header>+;

#pragma link C++ class dataProducts::ChannelHeader+;
#pragma link C++ class std::vector<dataProducts::ChannelHeader>+;

#pragma link C++ class dataProducts::WaveformHeader+;
#pragma link C++ class std::vector<dataProducts::WaveformHeader>+;

#pragma link C++ class dataProducts::Waveform+;
#pragma link C++ class std::vector<dataProducts::Waveform>+;

#pragma link C++ class dataProducts::FC7Header+;
#pragma link C++ class std::vector<dataProducts::FC7Header>+;

#endif
