## CONTENTS
- [OVERVIEW](#overview)
- [API DESCRIPTION](#api-description)
  - [getVersion(..)](#getversion)
  - [createTracer(..)](#createtracer)
  - [print(..)](#print)
- [USAGE EXAMPLE](#usage-example)

## OVERVIEW

This project helps implement bug tracking. A little explanation:

When creating a tracer, you will need to specify a name for it and select the minimum level for outputting messages (this level is private and can be changed using the `setLevel` method)

Example:

```cpp
std::shared_ptr<Tracer> warning = Tracer::createTracer(WARNING,"Warning");
```

This tracer will output `WARNING` messages and above (`EXCEPTION` and `CRITICAL`), i.e. if you try to print a message at the `DEBUG` level, it will be ignored.

Example:
```cpp
std::shared_ptr<Tracer> warning = Tracer::createTracer(WARNING,"Warning");

warning->print(DEBUG)     << "Debug level"     << std::endl;
warning->print(INFO)      << "Info level"      << std::endl;
warning->print(WARNING)   << "Warning level"   << std::endl;
warning->print(EXCEPTION) << "Exception level" << std::endl;
warning->print(CRITICAL)  << "Critical level"  << std::endl;
```

Below is an example output (Debug and Info levels will be skipped):

```
[WARNING  ] Warning level
[EXCEPTION] Exception level
[CRITICAL ] Critical level
```

It was about the private level and the message level. But the tracer class also has the ability to set the global trace level - method `Tracer::setTraceLevel(TraceLevel globallevel)`. All messages below this level will be ignored. Example:

```cpp
std::shared_ptr<Tracer> warning = Tracer::createTracer(WARNING,"Warning");

// This means that all messages below the global level from all tracers will be additionally ignored.
Tracer::setTraceLevel(EXCEPTION);

warning->print(DEBUG)     << "Debug level"     << std::endl;
warning->print(INFO)      << "Info level"      << std::endl;
warning->print(WARNING)   << "Warning level"   << std::endl;
warning->print(EXCEPTION) << "Exception level" << std::endl;
warning->print(CRITICAL)  << "Critical level"  << std::endl;
```

Below is an example output (Debug and Info levels will be skipped):

```
[EXCEPTION] Exception level
[CRITICAL ] Critical level
```

## API DESCRIPTION

The **Tracer** contains one main class `Tracer` which can be used 
to simplify bug tracking. 

### getVersion(..)

```cpp
/**
* @brief Method to get string of current version of library.
*
* @return String of current library version.
*/
static std::string getVersion();
```

##### Description

*Static method to get string of current version of library.*

### createTracer(..)

```cpp
/**
 * @brief Method to create a tracer
 *
 * @param level Minimum trace level at which messages will be printed
 * @param name Tracer name
 *
 * @return Tracer object
 */
static std::shared_ptr<Tracer> createTracer(TraceLevel level, std::string name);
```

##### Description

*Static method to create tracer.*

### print(..)

```cpp
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
```

##### Description

*Methods to prints the received message through the operator "<<".*

## USAGE EXAMPLE

```cpp
#include <iostream>
#include <map>

#include "Tracer.h"

using namespace cr::utils;

// Entry point.
int main(void)
{
    std::cout<<"=================================================" << std::endl;
    std::cout<<"TracerExample " << Tracer::getVersion()            << std::endl;
    std::cout<<"=================================================" << std::endl;
    std::cout<<std::endl;

    // Prints all messages from DEBUG level and above
    std::shared_ptr<Tracer> tracer = Tracer::createTracer(DEBUG,"Main");

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
}
```