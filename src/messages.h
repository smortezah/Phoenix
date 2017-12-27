#ifndef PHOENIX_MESSAGES_H
#define PHOENIX_MESSAGES_H

#include <iostream>


class Messages
{
public:
    Messages () = default;                                 // Constructor
    
    void help    () const;                                 // Usage guide
    void verbose () const;                                 // Verbose mode
};

#endif //PHOENIX_MESSAGES_H