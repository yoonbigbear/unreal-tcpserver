#ifndef _ENUMS_H_
#define _ENUMS_H_

enum class result_code : unsigned short {
    kCreateSuccess,
    kCreateFailed,

    kAleadyExist,
    kNotExist,

    kLoginSuccess,
    kLoginFailed,
};

enum class character_class : unsigned char {
    
    kNone,

    kNovice,

    kEnd,
};


#endif // !_ENUMS_H_
