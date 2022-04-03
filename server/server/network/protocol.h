#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

enum class protocol : unsigned short {
    kTextSend,
    kLogin,
    kCreateAccount,
    kCreateCharacter,
    kCheckCharacterNickname,

    kResultCode
};


#endif // ! _PROTOCOL_H_