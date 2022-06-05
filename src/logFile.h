#pragma once
#include <string>
#include <utility>
#include "exceptions.h"

/// sends strings to file in /assets/logs/logfile.txt
/// @param level specifies criticality of given message (higher means most critical)
/// @param text string to be printed as message in log
/// @param context string specifying who called this logWriting
bool sendToLogFile(short level, const std::string& text, const std::string& context);


