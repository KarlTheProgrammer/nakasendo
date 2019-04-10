#ifndef __MESSAGE_HASH_H__
#define __MESSAGE_HASH_H__

#include <memory>

//using MsgHashPtr = std::unique_ptr<unsigned char> ; 

class MessageHashImpl;

class MessageHash
{
    public:
        explicit MessageHash () ; 
        ~MessageHash ();

        void HashSha256(const std::string&);
        void Hash(const std::string&, const std::string&);
        std::string HashHex (); 


        void printHash () ; 
        std::string ListAvailableHash () ; 

    private:

        MessageHash (const MessageHash& ) ; 
        MessageHash& operator= (const MessageHash& ) ; 

        MessageHash (const MessageHash&& ) ; 
        MessageHash& operator= (const MessageHash&& ) ; 
        std::unique_ptr<MessageHashImpl> m_pImpl ; 
};

#endif //ifndef __MESSAGE_HASH_H__