#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <iostream>
#include <vector>

#include <flatbuffers/flatbuffers.h>

#pragma warning(disable:4996)
#pragma comment(lib, "flatbuffers.lib")

namespace net
{
    template<typename T>
    struct MessageHeader
    {
        T id;
        uint32_t size = 0;
    };

    template<typename T>
    struct Message
    {
        MessageHeader<T> header;
        std::vector<uint8_t> body;

        size_t size() const
        {
            return body.size();
        }

        friend std::ostream& operator << (std::ostream& os, const Message<T>& msg)
        {
            os << "ID:" << static_cast<int>(msg.header.id) << " Size:" << msg.header.size;
            return os;
        }

        template<typename DataType>
        friend Message<T>& operator << (Message<T>& msg, const DataType& data)
        {
            static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

            size_t i = msg.body.size();

            msg.body.resize(msg.body.size() + sizeof(DataType));

            std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

            msg.header.size = static_cast<uint32_t>(msg.size());

            return msg;
        }

        friend Message<T>& operator << (Message<T>& msg, const flatbuffers::FlatBufferBuilder& data)
        {
            static_assert(std::is_standard_layout<flatbuffers::FlatBufferBuilder>::value, "Data is too complex to be pushed into vector");

            size_t i = msg.body.size();

            msg.body.resize(msg.body.size() + data.GetSize());

            std::memcpy(msg.body.data() + i, data.GetBufferPointer(), data.GetSize());

            msg.header.size = static_cast<uint32_t>(msg.size());

            return msg;
        }

        template<typename DataType>
        friend Message<T>& operator >> (Message<T>& msg, DataType& data)
        {
            static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

            size_t i = msg.body.size() - sizeof(DataType);

            std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

            msg.body.resize(i);

            msg.header.size = static_cast<uint32_t>(msg.size());

            return msg;
        }
    };

    template <typename T>
    class Session;

    template <typename T>
    struct OwnedMessage
    {
        std::shared_ptr<Session<T>> remote = nullptr;
        Message<T> msg;

        friend std::ostream& operator<<(std::ostream& os, const OwnedMessage<T>& msg)
        {
            os << msg.msg;
            return os;
        }
    };
} // namespace net

#endif // _MESSAGE_H_