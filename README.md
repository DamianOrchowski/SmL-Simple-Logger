## Simple Logger 

It's a simple library used to:
- debug you code
- log actions


## Usage:
That code logs a "Hello World!" message to a file
```c++
#include <Logger.hpp>

int main() {

    LOG("Hello World!");
    return 0;
}
```
---
To log a warning use: 
```c++
LOG_WARNING("That's a warning!");
```
---
To log an error and critical error use:
```c++
LOG_ERROR("That's an error!");
LOG_CRITICAL("That's a critical error!");
```
## Log File customization
There's not many you can do to customize your
log file, but at least there's anything. You 
can set log file openning message and enable 
logging date in your files.
```c++

```
