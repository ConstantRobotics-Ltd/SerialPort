#include <iostream>
#include <map>

#include "Tracer.h"

using namespace cr::utils;

std::shared_ptr<Tracer> tracer;
std::shared_ptr<Tracer> debug;
std::shared_ptr<Tracer> info;
std::shared_ptr<Tracer> warning;
std::shared_ptr<Tracer> exception;
std::shared_ptr<Tracer> critical;

void longTrace(TraceLevel level);
void shortTrace(TraceLevel level);
void simplePrint(TraceLevel level);
void colorPrint();
void colorPrintWithHead();

// For different purposes (save to a file, send debug information to another
// application, etc.)
void testCallback(TraceLevel lvl, std::string msg, std::string tracerName,
          TraceColor color, std::string headerName)
{
    //std::cout << "Callback: "<< headerName << " " << tracerName << ": "<< msg;
    (void)lvl; (void)msg; (void)tracerName; (void)color; (void)headerName;
}

// Entry point.
int main(void)
{
    std::cout<<"=================================================" << std::endl;
    std::cout<<"TracerExample " << Tracer::getVersion()            << std::endl;
    std::cout<<"=================================================" << std::endl;
    std::cout<<std::endl;

    tracer = Tracer::createTracer(DEBUG,"Main");
    debug = Tracer::createTracer(DEBUG,"Debug");
    info = Tracer::createTracer(INFO,"Info");
    warning = Tracer::createTracer(WARNING,"Warning");
    exception = Tracer::createTracer(EXCEPTION,"Exception");
    critical = Tracer::createTracer(CRITICAL,"Critical");

    tracer->setFile("Main.txt");
    debug->setFile("General.txt");
    info->setFile("General.txt");
    warning->setFile("General.txt");
    exception->setFile("General.txt");
    critical->setFile("General.txt");

    std::cout<< "Long Trace: " << std::endl;
    std::cout<<"=================================================" << std::endl;
    TRACE(tracer, LONG_PRINT, WARNING, "%s\n", "WARNING message");
    std::cout<<"=================================================" << std::endl;
    std::cout << std::endl;

    std::cout<<"Short Trace: " << std::endl;
    std::cout<<"=================================================" << std::endl;
    TRACE(tracer, SHORT_PRINT, EXCEPTION, "%s\n", "EXCEPTION message");
    std::cout<<"=================================================" << std::endl;
    std::cout << std::endl;

    std::cout<<"Simple Print: " << std::endl;
    std::cout<<"=================================================" << std::endl;
    tracer->print(INFO) << "INFO message" << std::endl;
    std::cout<<"=================================================" << std::endl;
    std::cout << std::endl;

    std::cout<<"Color Print without Head: " << std::endl;
    std::cout<<"=================================================" << std::endl;
    tracer->print(MAGENTA) << "MAGENTA color message without head" << std::endl;
    std::cout<<"=================================================" << std::endl;
    std::cout << std::endl;

    std::cout<<"Color Print with Head: " << std::endl;
    std::cout<<"=================================================" << std::endl;
    tracer->print(RED, "RED") << "Simple message with color head"  << std::endl;
    std::cout<<"=================================================" << std::endl;
    std::cout << std::endl;

    const static std::map<int, TraceLevel> levels = {
        std::make_pair(1, DEBUG),
        std::make_pair(2, INFO),
        std::make_pair(3, WARNING),
        std::make_pair(4, EXCEPTION),
        std::make_pair(5, CRITICAL)
    };

    Tracer::setPrintCallback(testCallback);

    int globalLevel = 1;
    int messageLevel = 1;
    while (true)
    {
        std::cout << "DEBUG = 1, INFO = 2, WARNING = 3, "
                     "EXCEPTION = 4, CRITICAL = 5" << std::endl;
        std::cout << "Select global trace level (): ";
        std::cin >> globalLevel;
        if (levels.count(globalLevel))
            Tracer::setTraceLevel(levels.at(globalLevel));
        else
        {
            std::cout << "Wrong global level selected!" << std::endl;
            continue;
        }

        std::cout << "DEBUG = 1, INFO = 2, WARNING = 3, "
                     "EXCEPTION = 4, CRITICAL = 5" << std::endl;
        std::cout << "Select message trace level: ";
        std::cin >> messageLevel;
        if (levels.count(messageLevel))
        {
            longTrace(levels.at(messageLevel));
            shortTrace(levels.at(messageLevel));
            simplePrint(levels.at(messageLevel));
            colorPrint();
            colorPrintWithHead();
        }else
        {
            std::cout << "Wrong message level selected!" << std::endl;
            continue;
        }

    }


	return 1;
}

void longTrace(TraceLevel level)
{
    std::cout << std::endl << "Long Trace: " << std::endl;
    std::cout<<"=================================================" << std::endl;
    TRACE(debug, LONG_PRINT, level, "%s\n", "Debug traser");
    TRACE(info, LONG_PRINT, level, "%s\n", "Info traser");
    TRACE(warning, LONG_PRINT, level, "%s\n", "Warning traser");
    TRACE(exception, LONG_PRINT, level, "%s\n", "Exception traser");
    TRACE(critical, LONG_PRINT, level, "%s\n", "Critical traser");
    std::cout<<"=================================================" << std::endl;
    std::cout << std::endl;
}

void shortTrace(TraceLevel level)
{
    std::cout<<"Short Trace: " << std::endl;
    std::cout<<"=================================================" << std::endl;
    TRACE(debug, SHORT_PRINT, level, "%s\n", "Debug traser");
    TRACE(info, SHORT_PRINT, level, "%s\n", "Info traser");
    TRACE(warning, SHORT_PRINT, level, "%s\n", "Warning traser");
    TRACE(exception, SHORT_PRINT, level, "%s\n", "Exception traser");
    TRACE(critical, SHORT_PRINT, level, "%s\n", "Critical traser");
    std::cout<<"=================================================" << std::endl;
    std::cout << std::endl;
}

void simplePrint(TraceLevel level)
{
    std::cout<<"Simple Print: " << std::endl;
    std::cout<<"=================================================" << std::endl;
    debug->print(level) << "Debug traser" << std::endl;
    info->print(level) << "Info traser" << std::endl;
    warning->print(level) << "Warning traser" << std::endl;
    exception->print(level) << "Exception traser" << std::endl;
    critical->print(level) << "Critical traser" << std::endl;
    std::cout<<"=================================================" << std::endl;
    std::cout << std::endl;
}

void colorPrint()
{
    std::cout<<"Color Print: " << std::endl;
    std::cout<<"=================================================" << std::endl;
    debug->print(GREEN) << "Debug traser" << std::endl;
    info->print(CYAN) << "Info traser" << std::endl;
    warning->print(RED) << "Warning traser" << std::endl;
    exception->print(MAGENTA) << "Exception traser" << std::endl;
    critical->print(YELLOW) << "Critical traser" << std::endl;
    std::cout<<"=================================================" << std::endl;
    std::cout << std::endl;
}

void colorPrintWithHead()
{
    std::cout<<"Color Print With Head: " << std::endl;
    std::cout<<"=================================================" << std::endl;
    debug->print(GREEN, "GREEN") << "Debug traser" << std::endl;
    info->print(CYAN, "CYAN") << "Info traser" << std::endl;
    warning->print(RED, "RED") << "Warning traser" << std::endl;
    exception->print(MAGENTA, "MAGENTA") << "Exception traser" << std::endl;
    critical->print(YELLOW, "YELLOW") << "Critical traser" << std::endl;
    std::cout<<"=================================================" << std::endl;
    std::cout << std::endl;
}
