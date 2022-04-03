#ifndef _SAMPLEPACKET_HELPER_CC_
#define _SAMPLEPACKET_HELPER_CC_


#include <SamplePacket_generated.h>
#include "../protocol.h"
#include "../../enums.h"

namespace SamplePacketHelper {

    using namespace flatbuffers;
    using namespace SamplePacket;

    template<typename T>
    static void TextAck(T& msg, const std::string_view& str)
    {
       msg.header.id = protocol::kTextSend;
       FlatBufferBuilder builder(1024);
       auto text = builder.CreateString(str);
       textREQBuilder text_req_builder(builder);
       text_req_builder.add_text(text);
       auto ack = text_req_builder.Finish();
       builder.Finish(ack);
       msg << builder;
    }

    template<typename T>
    static void ResultCodeAck(T& msg, result_code result_code)
    {
        msg.header.id = protocol::kResultCode;
        FlatBufferBuilder builder(1024);
        ResultCodeBuilder res_code_builder(builder);
        res_code_builder.add_rescode(static_cast<uint16_t>(result_code));
        auto fbb = res_code_builder.Finish();
        builder.Finish(fbb);
        msg << builder;
    }

};
#endif // !_SAMPLEPACKET_HELPER_CC_