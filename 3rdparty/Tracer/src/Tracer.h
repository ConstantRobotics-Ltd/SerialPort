#pragma once
#include <thread>
#include <mutex>
#include <string.h>
#include <sstream>
#include <memory>
#include <vector>
#include <stdio.h>


#if defined(linux) || defined(__linux) || defined(__linux__)
#define __FILENAME__ (strrchr(__FILE__,'/')?strrchr(__FILE__,'/')+1:__FILE__)
#define sprintf_s(buf, size, ...) snprintf(buf, size, __VA_ARGS__)
#else
#include <stdio.h>

#define __FILENAME__ (strrchr(__FILE__,'\\')?strrchr(__FILE__,'\\')+1:__FILE__)
#endif

#ifndef NDEBUG
#define TRACE(tracer, traceFormat, lvl, printFormat, ... ) \
if (traceFormat == SHORT_PRINT) \
{ TRACE_SHORT(tracer, lvl, printFormat, __VA_ARGS__); } \
else { TRACE_LONG(tracer, lvl, printFormat, __VA_ARGS__); }
#else
#define TRACE(tracer, traceFormat, lvl, printFormat, ... ) ((void)0)
#endif

#ifndef NDEBUG
#define TRACE_LONG(tracer, lvl, format, ... ) \
if (lvl <= Tracer::getTraceLevel() && lvl <= tracer->getLevel()) \
{ Tracer::m_printMutex.lock(); switch(lvl){ \
    case CRITICAL: \
{   char buffer [1024]; \
    sprintf_s (buffer, 1024, "" format, __VA_ARGS__); \
    tracer->print(CRITICAL) << buffer; \
    memset(buffer, 0, 1024); \
    sprintf_s (buffer, 1024, "%s()\n",  __FUNCTION__ ); \
    tracer->print(CRITICAL, "---------") << buffer; \
    memset(buffer, 0, 1024); \
    sprintf_s (buffer, 1024, "%s:%d\n", __FILENAME__, __LINE__ ); \
    tracer->print(CRITICAL, "---------") << buffer; \
    memset(buffer, 0, 1024); \
    break; } \
    case EXCEPTION: \
{   char buffer [1024]; \
    sprintf_s (buffer, 1024, "" format, __VA_ARGS__); \
    tracer->print(EXCEPTION) << buffer; \
    memset(buffer, 0, 1024); \
    sprintf_s (buffer, 1024, "%s()\n",  __FUNCTION__ ); \
    tracer->print(EXCEPTION, "---------") << buffer; \
    memset(buffer, 0, 1024); \
    sprintf_s (buffer, 1024, "%s:%d\n", __FILENAME__, __LINE__ ); \
    tracer->print(EXCEPTION, "---------") << buffer; \
    memset(buffer, 0, 1024); \
    break; } \
    case WARNING: \
{   char buffer [1024]; \
    sprintf_s (buffer, 1024, "" format, __VA_ARGS__); \
    tracer->print(WARNING) << buffer; \
    memset(buffer, 0, 1024); \
    sprintf_s (buffer, 1024, "%s()\n",  __FUNCTION__ ); \
    tracer->print(WARNING, "---------") << buffer; \
    memset(buffer, 0, 1024); \
    sprintf_s (buffer, 1024, "%s:%d\n", __FILENAME__, __LINE__ ); \
    tracer->print(WARNING, "---------") << buffer; \
    memset(buffer, 0, 1024); \
    break; } \
    case INFO: \
{   char buffer [1024]; \
    sprintf_s (buffer, 1024, "" format, __VA_ARGS__); \
    tracer->print(INFO) << buffer; \
    memset(buffer, 0, 1024); \
    sprintf_s (buffer, 1024, "%s()\n",  __FUNCTION__ ); \
    tracer->print(INFO, "---------") << buffer; \
    memset(buffer, 0, 1024); \
    sprintf_s (buffer, 1024, "%s:%d\n", __FILENAME__, __LINE__ ); \
    tracer->print(INFO, "---------") << buffer; \
    memset(buffer, 0, 1024); \
    break; } \
    case DEBUG: \
{   char buffer [1024]; \
    sprintf_s (buffer, 1024, "" format, __VA_ARGS__); \
    tracer->print(DEBUG) << buffer; \
    memset(buffer, 0, 1024); \
    sprintf_s (buffer, 1024, "%s()\n",  __FUNCTION__ ); \
    tracer->print(DEBUG, "---------") << buffer; \
    memset(buffer, 0, 1024); \
    sprintf_s (buffer, 1024, "%s:%d\n", __FILENAME__, __LINE__ ); \
    tracer->print(DEBUG, "---------") << buffer; \
    memset(buffer, 0, 1024); \
    break; } \
    default: \
{   break; } \
} Tracer::m_printMutex.unlock(); \
};
#else
#define TRACE(tracer, lvl, format, ... ) ((void)0)
#endif

#ifndef NDEBUG
#define TRACE_SHORT(tracer, lvl, format, ... ) \
if (lvl <= Tracer::getTraceLevel() && lvl <= tracer->getLevel()) \
{   Tracer::m_printMutex.lock(); \
    char buffer [1024]; \
    int n = sprintf_s (buffer, 1024, "%s:%d: ", __FILENAME__, __LINE__ ); \
    sprintf_s (&buffer[n], 1024-n, "" format, __VA_ARGS__ ); \
    tracer->print(lvl) << buffer; \
    Tracer::m_printMutex.unlock(); \
};
#else
#define TRACE(tracer, lvl, format, ... ) ((void)0)
#endif

namespace cr {
namespace utils {

enum TraceColor
{
    GREEN = 10,
    CYAN = 11,
    RED = 12,
    MAGENTA = 13,
    YELLOW = 14,
    WHITE = 15
};

enum TraceLevel
{
    SILENT = -1,
    CUSTOM = 0,
    CRITICAL = 1,
    EXCEPTION = 2,
    WARNING = 3,
    INFO = 4,
    DEBUG = 5
};

enum TraceFormat
{
    SHORT_PRINT = 0,
    LONG_PRINT = 1
};


typedef void(*printCallback)(TraceLevel lvl, std::string msg, std::string tracerName,
                             TraceColor color, std::string headerName);

class ColorPrint
{
public:
    ColorPrint(TraceColor color, std::string tracerName, std::string traceHeader, printCallback clb = nullptr, std::string file = "");
    ColorPrint(TraceLevel level, std::string tracerName, std::string traceHeader = "", printCallback clb = nullptr, std::string file = "");
    /**
     * @brief Default destructor
     *
     * The destructor prints all messages, that were added by << operator,
     * before the tracer are destroyed
     */
    ~ColorPrint();
    /**
     * @brief Add message to the print
     * @tparam T type of the data to print
     * @param msg The message to print
     * @return
     */
    template <typename T>
    std::ostringstream& operator << (T msg) {
        *m_stream << msg;
        return *m_stream;
    }

    void print(TraceLevel level, const std::string &msg, std::string tracerName, std::string tracerHeader = "");
    void print(TraceColor color, const std::string &msg, std::string tracerName, std::string tracerHeader);

    static void colorize(const char* s, int color);

private:
    std::ostringstream* m_stream;
    TraceLevel m_printLevel;
    TraceColor m_printColor;
    printCallback m_printCallback;
    std::string m_printFileName;
    std::string m_tracerName;
    std::string m_traceHeader;
};

/**
 * @class
 *
 * @brief Implement tracing
 */
class Tracer
{
public:

    /**
     * @brief Method to get string of current version of library.
     *
     * @return String of current library version.
     */
    static std::string getVersion();

    /**
     * @brief Class constructor.
     *
     * @param level Minimum trace level at which messages will be printed
     * @param name Tracer name.
     */
    Tracer(TraceLevel level, std::string name);

    /**
     * @brief Class destructor.
     */
    ~Tracer();

    /**
     * @brief Method for setting the lowest trace level for a specific tracer
     *
     * @param level Minimum trace level at which messages will be printed
     * @param name Tracer name. If name is empty, trace level will be set for
     * all tracers.
     *
     * @return true if a tracer was found by name
     */
    static bool setTraceLevel(TraceLevel level, std::string name = "");

    /**
     * @brief Method for setting print callback a specific tracer
     *
     * @param clb Callback
     * @param name Tracer name. If name is empty, callback function will be set
     * for all tracers.
     *
     * @return true if a tracer was found by name
     */
    static bool setPrintCallback(printCallback clb, std::string name = "");

    /**
     * @brief Method for setting file for a specific tracer
     *
     * @param filename The name of the file where the log will be printed.
     * @param name Tracer name. If name is empty, this file will be set for all
     * tracers.
     *
     * @return true if a tracer was found by name
     */
    static bool setPrintFile(std::string filename, std::string name = "");

    /**
     * @brief Method for getting trace level from a specific tracer
     *
     * @param name Tracer name. If name is empty, will be return global level
     *
     * @return Tracer level
     */
    static TraceLevel getTraceLevel(std::string name = "");

    /**
     * @brief Method for getting print callback from a specific tracer
     *
     * @param name Tracer name. If name is empty, will be return global callback
     *
     * @return callback
     */
    static printCallback getPrintCallback(std::string name = "");

    /**
     * @brief Method for getting print file from a specific tracer
     *
     * @param name Tracer name. If name is empty, will be return global callback
     *
     * @return callback
     */
    static std::string getPrintFile(std::string name = "");

    /**
     * @brief Method for creating a tracer that will be added to the internal
     * static list (it is necessary that access to this tracer be available
     * by name from any sources)
     *
     * @param level Minimum trace level at which messages will be printed
     * @param name Tracer name
     *
     * @return Tracer object
     */
    static std::shared_ptr<Tracer> createTracer(TraceLevel level, std::string name);

    /**
     * @brief Methods to prints the received message through the operator "<<"
     *
     * @param msgLevel Message importance level. If it is higher or equal to
     * the minimum level of the tracer, then the message will be printed
     *
     * @return object that, when destroyed, will output a message to the stream.
     */
    ColorPrint print(TraceLevel msgLevel, std::string traceHeader = "");
    ColorPrint print(TraceColor msgColor, std::string traceHeader = "");

    /**
     * @brief setTraceLevel - Sets the lowest trace level
     * @param level Minimum trace level at which messages will be printed
     */
    void setLevel(TraceLevel level);

    void setCallback(printCallback clb);
    printCallback getCallback();

    void setFile(std::string filename);
    std::string getFile();

    /**
     * @brief getTraceLevel - Get current trace level
     * @return
     */
    TraceLevel getLevel();

    /**
     * @brief getName Tracer name
     * @return name
     */
    std::string getName();
    static std::mutex m_printMutex;

private:

    static TraceLevel m_globallevel;
    static printCallback m_globalPrint;
    static std::string m_globalFile;
    static std::vector<std::shared_ptr<Tracer>> tracers;
    TraceLevel m_currentLevel;
    std::string m_name;
    printCallback m_print;
    std::string m_fileName;
};
}
}

