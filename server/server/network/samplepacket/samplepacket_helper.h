#ifndef _SAMPLEPACKET_HELPER_CC_
#define _SAMPLEPACKET_HELPER_CC_

#include <result_code_generated.h>
#include <protocol_generated.h>

namespace SamplePacketHelper {

    using namespace flatbuffers;

    template<typename T>
    static void TextAck(T& msg, const std::string_view& str)
    {
        //msg.header.id = Protocol_TextSend;
        //FlatBufferBuilder builder(1024);
        //auto text = builder.CreateString(str);
        //textREQBuilder text_req_builder(builder);
        //text_req_builder.add_text(text);
        //auto ack = text_req_builder.Finish();
        //builder.Finish(ack);
        //msg << builder;
    }

    template<typename T>
    static void ResultCodeAck(T& msg, ResultCode result_code)
    {
        //msg.header.id = Protocol_ResultCode;
        //FlatBufferBuilder builder(1024);
        //ResultBuilder res_code_builder(builder);
        //res_code_builder.add_rescode(result_code);
        //auto fbb = res_code_builder.Finish();
        //builder.Finish(fbb);
        //msg << builder;
    }

};
#endif // !_SAMPLEPACKET_HELPER_CC_