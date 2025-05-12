#include "FC7Header.hh"

using namespace dataProducts;

FC7Header::FC7Header()
    : DataProduct(),
    crateNum(0),
    amcNum(0),
    triggerNum(0),
    clockCounter(0),
    fillType(0),
    boardType(0),
    fc7Type(0),
    boardID(0),
    xadcAlarms(0),
    emptyEvent(false),
    majorRevision(0),
    minorRevision(0),
    patchRevision(0)
{};

FC7Header::FC7Header(unsigned int crateNumArg
    ,unsigned int amcNumArg
    ,unsigned int triggerNumArg
    ,unsigned long clockCounterArg
    ,unsigned int fillTypeArg
    ,unsigned int boardTypeArg
    ,unsigned int fc7TypeArg
    ,unsigned int boardIDArg
    ,unsigned char xadcAlarmsArg
    ,bool emptyEventArg
    ,unsigned int majorRevisionArg
    ,unsigned int minorRevisionArg
    ,unsigned patchRevisionArg)
    : DataProduct(),
    crateNum(0),
    amcNum(0),
    triggerNum(0),
    clockCounter(0),
    fillType(0),
    boardType(0),
    fc7Type(0),
    boardID(0),
    xadcAlarms(0),
    emptyEvent(false),
    majorRevision(0),
    minorRevision(0),
    patchRevision(0)
{};

FC7Header::~FC7Header() {};

void FC7Header::Show() const {
    std::ostringstream oss;
    oss << std::endl;
    oss << "FC7Header:" << std::endl;
    oss << "    crateNum: " << crateNum  << std::endl;
    oss << "    amcNum: " << amcNum  << std::endl;
    oss << "    triggerNum: " << triggerNum  << std::endl;
    oss << "    clockCounter: " << clockCounter  << std::endl;
    oss << "    fillType: " << fillType  << std::endl;
    oss << "    boardType: " << boardType  << std::endl;
    oss << "    fc7Type: " << fc7Type  << std::endl;
    oss << "    boardID: " << boardID  << std::endl;
    oss << "    xadcAlarms: " << xadcAlarms  << std::endl;
    oss << "    emptyEvent: " << (emptyEvent ? "true" : "false")  << std::endl;
    oss << "    majorRevision: " << majorRevision  << std::endl;
    oss << "    minorRevision: " << minorRevision  << std::endl;
    oss << "    patchRevision: " << patchRevision << std::endl;
    std::cout << oss.str();
}