#ifndef HANDLER_H
#define HANDLER_H

#include <string>
#include <functional>

class Handler
{
public:
    Handler();

    Handler(std::function <void(std::string)> handler);

    void execute(std::string objectName);

private:
    std::function<void(std::string)> handler;
};

#endif // HANDLER_H
