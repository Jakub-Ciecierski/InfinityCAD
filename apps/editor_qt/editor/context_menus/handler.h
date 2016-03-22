#ifndef HANDLER_H
#define HANDLER_H

#include <string>

class Handler
{
public:
    Handler();
    Handler(void (*handler)(std::string));

    void execute(std::string objectName);

private:
    void (*handler)(std::string);
};

#endif // HANDLER_H
