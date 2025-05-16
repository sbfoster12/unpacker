#if defined(__ROOTCLING__)

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class dataProducts::NaluEventHeader+;
#pragma link C++ class std::vector<dataProducts::NaluEventHeader>+;

#pragma link C++ class dataProducts::NaluPacketHeader+;
#pragma link C++ class std::vector<dataProducts::NaluPacketHeader>+;

#pragma link C++ class dataProducts::NaluPacket+;
#pragma link C++ class std::vector<dataProducts::NaluPacket>+;

#pragma link C++ class dataProducts::NaluPacketFooter+;
#pragma link C++ class std::vector<dataProducts::NaluPacketFooter>+;

#pragma link C++ class dataProducts::NaluWaveform+;
#pragma link C++ class std::vector<dataProducts::NaluWaveform>+;

#pragma link C++ class dataProducts::NaluEventFooter+;
#pragma link C++ class std::vector<dataProducts::NaluEventFooter>+;

#pragma link C++ class dataProducts::NaluTime+;
#pragma link C++ class std::vector<dataProducts::NaluTime>+;

#pragma link C++ class dataProducts::NaluODB+;
#pragma link C++ class std::vector<dataProducts::NaluODB>+;

#endif
