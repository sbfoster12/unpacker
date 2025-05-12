#include "WFD5Header.hh"

using namespace dataProducts;

WFD5Header::WFD5Header()
    : DataProduct(),
    crateNum(0),
    amcNum(0),
    triggerNum(0),
    clockCounter(0),
    fillType(0),
    boardType(0),
    boardID(0),
    xadcAlarms(0),
    emptyEvent(false),
    majorRevision(0),
    minorRevision(0),
    patchRevision(0)
{}

WFD5Header::WFD5Header(
    unsigned int crateNumArg
    ,unsigned int amcNumArg
    ,unsigned int triggerNumArg
    ,unsigned long clockCounterArg
    ,unsigned int fillTypeArg
    ,unsigned int boardTypeArg
    ,unsigned int boardIDArg
    ,unsigned char xadcAlarmsArg
    ,bool emptyEventArg
    ,unsigned int majorRevisionArg
    ,unsigned int minorRevisionArg
    ,unsigned int patchRevisionArg
    )
    : DataProduct(),
    crateNum(crateNumArg),
    amcNum(amcNumArg),
    triggerNum(triggerNumArg),
    clockCounter(clockCounterArg),
    fillType(fillTypeArg),
    boardType(boardTypeArg),
    boardID(boardIDArg),
    xadcAlarms(xadcAlarmsArg),
    emptyEvent(emptyEventArg),
    majorRevision(majorRevisionArg),
    minorRevision(minorRevisionArg),
    patchRevision(patchRevisionArg)
{}

WFD5Header::~WFD5Header() {};

void WFD5Header::Show() const {
    std::ostringstream oss;
    oss << std::endl;
    oss << "WFD5Header: " << crateNum << std::endl;
    oss << "    crateNum: " << crateNum << std::endl;
    oss << "    amcNum: " << amcNum << std::endl;
    oss << "    triggerNum: " << triggerNum << std::endl;
    oss << "    clockCounter: " << clockCounter << std::endl;
    oss << "    fillType: " << fillType << std::endl;
    oss << "    boardType: " << boardType << std::endl;
    oss << "    boardID: " << boardID << std::endl;
    oss << "    xadcAlarms: " << xadcAlarms << std::endl;
    oss << "    emptyEvent: " << (emptyEvent ? "true" : "false") << std::endl << std::endl;
    oss << "    majorRevision: " << majorRevision << std::endl;
    oss << "    minorRevision: " << minorRevision << std::endl;
    oss << "    patchRevision: " << patchRevision << std::endl;
    std::cout << oss.str();
}