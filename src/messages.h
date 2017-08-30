#ifndef PHOENIX_MESSAGES_H
#define PHOENIX_MESSAGES_H

#include <iostream>


class Messages
{
public:
    Messages () = default;                                 // constructor
    
    void help    () const;                                 // show usage guide
    void about   () const;                                 // show About Phoenix
    void verbose () const;                                 // show verbose mode
};

#endif //PHOENIX_MESSAGES_H