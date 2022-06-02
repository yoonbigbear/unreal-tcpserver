#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <iostream>
#include <boost/asio.hpp>

namespace net
{
    template<typename T>
    struct MessageHeader
    {
        T id;
        uint16_t size = 0;
    };

    template<typename T, typename U>
    struct Message
    {
        using message = Message<T, U>;

        MessageHeader<T> header;
        std::vector<uint8_t> body;

        size_t size() const
        {
            return body.size();
        }

        friend std::ostream& operator << (std::ostream& os, const message& msg)
        {
            os << "ID:" << static_cast<int>(msg.header.id) << " Size:" << msg.header.size;
            return os;
        }

        template<typename DataType>
        friend message& operator << (message& msg, const DataType& data)
        {
            static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

            size_t i = msg.body.size();

            msg.body.resize(msg.body.size() + sizeof(DataType));

            std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

            msg.header.size = static_cast<uint16_t>(msg.size());

            return msg;
        }

        friend message& operator << (message& msg, const U& data)
        {
            static_assert(std::is_standard_layout<U>::value, "Data is too complex to be pushed into vector");

            size_t i = msg.body.size();

            msg.body.resize(msg.body.size() + data.GetSize());

            std::memcpy(msg.body.data() + i, data.GetBufferPointer(), data.GetSize());

            msg.header.size = static_cast<uint16_t>(msg.size());

            return msg;
        }

        template<typename DataType>
        friend message& operator >> (message& msg, DataType& data)
        {
            static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

            size_t i = msg.body.size() - sizeof(DataType);

            std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

            msg.body.resize(i);

            msg.header.size = static_cast<uint16_t>(msg.size());

            return msg;
        }
    };

    template <typename T, typename U>
    class Session;

    template <typename T, typename U>
    struct OwnedMessage
    {
        std::shared_ptr<Session<T, U>> remote = nullptr;
        Message<T, U> msg;

        friend std::ostream& operator<<(std::ostream& os, const OwnedMessage<T, U>& msg)
        {
            os << msg.msg;
            return os;
        }
    };
} // namespace net

#endif // _MESSAGE_H_