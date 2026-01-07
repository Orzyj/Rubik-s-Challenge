#ifndef HISTORYLOGGER_H
#define HISTORYLOGGER_H

#include <vector>
#include "TMove.h"

class HistoryLogger {
    public:

    static bool exportAsCSV(std::vector<TMove> data);
    static bool exportAsJSON(std::vector<TMove> data);
};

#endif // HISTORYLOGGER_H
