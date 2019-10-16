#ifndef __MESSAGE_HASH_H__
#define __MESSAGE_HASH_H__

#include <memory>
#include <string>

#include <MessageHash/MessageHashConfig.h>

class MessageHashImpl;

class MessageHash_API MessageHash
{
    public:
        explicit MessageHash () ;
        ~MessageHash ();

        void HashSha256(const std::string&);
        void Hash(const std::string&, const std::string&);
        std::string HashHex (); 
        std::unique_ptr<unsigned char> Value (); 


        void printHash () ; 
        std::string ListAvailableHash () ;

    private:

        MessageHash (const MessageHash& ) ; 
        MessageHash& operator= (const MessageHash& ) ; 

        MessageHash (const MessageHash&& ) ; 
        MessageHash& operator= (const MessageHash&& ) ; 
        std::unique_ptr<MessageHashImpl> m_pImpl ; 
};

std::string MessageHash_API HashMsgSHA256(const std::string& crMsg);
#endif //ifndef __MESSAGE_HASH_H__