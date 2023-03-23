#include "Tracer.h"
#include "TracerVersion.h"

#include <string>
#include <algorithm>
#include <iostream>
#include <map>

#include <fstream>
#include <string>
#include <iostream>

#if defined(linux) || defined(__linux) || defined(__linux__)

#else
#include <windows.h>
#endif

using namespace cr::utils;

std::mutex Tracer::m_printMutex;
TraceLevel Tracer::m_globallevel = TraceLevel::DEBUG;
printCallback Tracer::m_globalPrint = nullptr;
std::string Tracer::m_globalFile = "";
std::vector<std::shared_ptr<Tracer>> Tracer::tracers;

std::string Tracer::getVersion()
{
    return COLOR_PRINT_VERSION;
}

bool Tracer::setTraceLevel(TraceLevel globallevel, std::string name)
{
    if (name.size() == 0)
    {
        m_globallevel = globallevel;
    }
    else
    {
        auto it = std::find_if(
                    tracers.begin(), tracers.end(),
                    [&name](std::shared_ptr<Tracer>& obj){return obj->getName() == name;});

        int index = -1;
        if (it != std::end(tracers))
        {
            index = (int)std::distance(tracers.begin(), it);
        }

        if (index == -1)
        {
            return false;
        }else
        {
            tracers[index]->setLevel(globallevel);
        }
    }
    return true;
}

TraceLevel Tracer::getTraceLevel(std::string name)
{
    if (name.size() == 0)
    {
         return m_globallevel;
    }
    else
    {
        auto it = std::find_if(
                    tracers.begin(), tracers.end(),
                    [&name](std::shared_ptr<Tracer>& obj){return obj->getName() == name;});

        int index = -1;
        if (it != std::end(tracers))
        {
            index = (int)std::distance(tracers.begin(), it);
        }

        if (index == -1)
        {
            return m_globallevel;
        }else
        {
            return tracers[index]->getLevel();
        }
    }
}

bool Tracer::setPrintCallback(printCallback clb, std::string name)
{
    if (name.size() == 0)
    {
        m_globalPrint = clb;
    }
    else
    {
        auto it = std::find_if(
                    tracers.begin(), tracers.end(),
                    [&name](std::shared_ptr<Tracer>& obj){return obj->getName() == name;});

        int index = -1;
        if (it != std::end(tracers))
        {
            index = (int)std::distance(tracers.begin(), it);
        }

        if (index == -1)
        {
            return false;
        }else
        {
            tracers[index]->setCallback(clb);
        }
    }
    return true;
}

bool Tracer::setPrintFile(std::string filename, std::string name)
{
    if (name.size() == 0)
    {
        m_globalFile = filename;
    }
    else
    {
        auto it = std::find_if(
                    tracers.begin(), tracers.end(),
                    [&name](std::shared_ptr<Tracer>& obj){return obj->getName() == name;});

        int index = -1;
        if (it != std::end(tracers))
        {
            index = (int)std::distance(tracers.begin(), it);
        }

        if (index == -1)
        {
            return false;
        }else
        {
            tracers[index]->setFile(filename);
        }
    }
    return true;
}

printCallback Tracer::getPrintCallback(std::string name)
{
    if (name.size() == 0)
    {
         return m_globalPrint;
    }
    else
    {
        auto it = std::find_if(
                    tracers.begin(), tracers.end(),
                    [&name](std::shared_ptr<Tracer>& obj){return obj->getName() == name;});

        int index = -1;
        if (it != std::end(tracers))
        {
            index = (int)std::distance(tracers.begin(), it);
        }

        if (index == -1)
        {
            return m_globalPrint;
        }else
        {
            return tracers[index]->getCallback();
        }
    }
}

std::string Tracer::getPrintFile(std::string name)
{
    if (name.size() == 0)
    {
         return m_globalFile;
    }
    else
    {
        auto it = std::find_if(
                    tracers.begin(), tracers.end(),
                    [&name](std::shared_ptr<Tracer>& obj){return obj->getName() == name;});

        int index = -1;
        if (it != std::end(tracers))
        {
            index = (int)std::distance(tracers.begin(), it);
        }

        if (index == -1)
        {
            return m_globalFile;
        }else
        {
            return tracers[index]->getFile();
        }
    }
}

std::shared_ptr<Tracer> Tracer::createTracer(TraceLevel level, std::string name)
{
    auto it = std::find_if(tracers.begin(), tracers.end(),
                           [&name](std::shared_ptr<Tracer>& obj) {return obj->getName() == name;});
    int index = -1;
    if (it != std::end(tracers))
    {
        index = (int)std::distance(tracers.begin(), it);
    }

    if (index == -1)
    {
        tracers.emplace_back(new Tracer(level, name));
        return tracers[tracers.size()-1];
    }else
    {
        tracers[index]->setLevel(level);
        return tracers[index];
    }
}

Tracer::Tracer(TraceLevel level, std::string name)
    : m_currentLevel(level)
    , m_name(name)
    , m_print(nullptr)
    , m_fileName("")
{
}

ColorPrint Tracer::print(TraceLevel currentLevel, std::string traceHeader)
{
    if (currentLevel <= m_currentLevel && currentLevel <= m_globallevel)
        return ColorPrint(currentLevel, m_name, traceHeader, getCallback(), getFile());
    else
        return ColorPrint(SILENT, m_name, traceHeader, getCallback(), getFile());
}

ColorPrint Tracer::print(TraceColor msgColor, std::string traceHeader)
{
    return ColorPrint(msgColor, m_name, traceHeader, getCallback(), getFile());
}

void Tracer::setLevel(TraceLevel level)
{
    m_currentLevel = level;
}

void Tracer::setCallback(printCallback clb)
{
    m_print = clb;
}

printCallback Tracer::getCallback()
{
    if (m_print != nullptr)
        return m_print;
    else
        return Tracer::getPrintCallback();
}

void Tracer::setFile(std::string filename)
{
    m_fileName = filename;
}

std::string Tracer::getFile()
{
    if (m_fileName != "")
        return m_fileName;
    else
        return Tracer::getPrintFile();
}

Tracer::~Tracer()
{
}

TraceLevel Tracer::getLevel()
{
    return m_currentLevel;
}

std::string Tracer::getName()
{
    return m_name;
}

ColorPrint::ColorPrint(TraceColor color, std::string tracerName, std::string traceHeader, printCallback clb, std::string file)
{
    m_printLevel = CUSTOM;
    m_printColor = color;
    m_stream = new std::ostringstream();
    m_tracerName = tracerName;
    m_traceHeader = traceHeader;
    m_printCallback = clb;
    m_printFileName = file;
}

ColorPrint::ColorPrint(TraceLevel level, std::string tracerName, std::string traceHeader, printCallback clb, std::string file)
{
    m_printLevel = level;
    m_stream = new std::ostringstream();
    m_tracerName = tracerName;
    m_traceHeader = traceHeader;
    m_printCallback = clb;
    m_printFileName = file;
}

ColorPrint::~ColorPrint()
{
    if (m_printLevel == CUSTOM) {
        print(m_printColor, (*m_stream).str(), m_tracerName, m_traceHeader);
    }
    else if (m_printLevel != SILENT) {
        print(m_printLevel, (*m_stream).str(), m_tracerName);
    }
    delete m_stream;
}

void ColorPrint::print(TraceLevel level, const std::string &msg, std::string tracerName, std::string tracerHeader)
{
    const static std::map<TraceLevel, std::string> LOGLEVEL_NAMES = {
            std::make_pair(CRITICAL, "[CRITICAL ] "),
            std::make_pair(EXCEPTION, "[EXCEPTION] "),
            std::make_pair(WARNING, "[WARNING  ] "),
            std::make_pair(INFO, "[INFO     ] "),
            std::make_pair(DEBUG, "[DEBUG    ] "),
    };

    const static std::map<TraceLevel, TraceColor> COLORLEVEL_VALUE = {
            std::make_pair(CRITICAL, RED),
            std::make_pair(EXCEPTION, RED),
            std::make_pair(WARNING, YELLOW),
            std::make_pair(INFO, CYAN),
            std::make_pair(DEBUG, WHITE),
    };

    if (tracerHeader == "")
    {
        colorize(LOGLEVEL_NAMES.at(level).c_str(), COLORLEVEL_VALUE.at(level));
        std::cout << msg;

        if(m_printCallback != nullptr)
            m_printCallback(level, msg, tracerName, COLORLEVEL_VALUE.at(level), LOGLEVEL_NAMES.at(level));

        if(m_printFileName != "")
        {
            std::ofstream out(m_printFileName, std::ios::app);
            out << "TRACER \"" << tracerName << "\": \t" << LOGLEVEL_NAMES.at(level) << " " << msg; out.close();
        }

    }else
    {
        std::string _tracerHeader("[" + tracerHeader + "] ");
        colorize(_tracerHeader.c_str(), COLORLEVEL_VALUE.at(level));

        std::cout << msg;

        if(m_printCallback != nullptr)
            m_printCallback(level, msg, tracerName, COLORLEVEL_VALUE.at(level), _tracerHeader);

        if(m_printFileName != "")
        {
            std::ofstream out(m_printFileName, std::ios::app);
            out << "TRACER \"" << tracerName << "\": \t" << _tracerHeader << " " << msg; out.close();
        }
    }


}

void ColorPrint::print(TraceColor color, const std::string &msg, std::string tracerName, std::string tracerHeader)
{
    if (tracerHeader!= "")
    {
        std::string _tracerHeader("[" + tracerHeader + "] ");
        colorize(_tracerHeader.c_str(), color);

        std::cout << msg << std::flush;

        if(m_printCallback != nullptr)
            m_printCallback(CUSTOM, msg, tracerName, color, tracerHeader);

        if(m_printFileName != "")
        {
            std::ofstream out(m_printFileName, std::ios::app);
            out << "TRACER \"" << tracerName << "\": \t" << _tracerHeader << " " << msg; out.close();
        }
    }else
    {
        colorize(msg.c_str(), color);

        std::cout << std::flush;

        if(m_printCallback != nullptr)
            m_printCallback(CUSTOM, msg, tracerName, color, tracerHeader);

        if(m_printFileName != "")
        {
            std::ofstream out(m_printFileName, std::ios::app);
            out << "TRACER \"" << tracerName << "\": \t" << msg; out.close();
        }
    }


}

void ColorPrint::colorize(const char *s, int color)
{
    Tracer::m_printMutex.lock();
#ifdef _WIN32
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | color);
    std::cout << s << std::flush;
    SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | WHITE);
#else
    switch (color) {
    case RED:
        printf("\033[;31m%s\033[0m", s);
        break;
    case GREEN:
        printf("\033[;32m%s\033[0m", s);
        break;
    case YELLOW:
        printf("\033[;33m%s\033[0m", s);
        break;
    case MAGENTA:
        printf("\033[;35m%s\033[0m", s);
        break;
    case CYAN:
        printf("\033[;36m%s\033[0m", s);
        break;
    default:
        printf("%s",s);
        break;
    }
#endif
    Tracer::m_printMutex.unlock();
}
