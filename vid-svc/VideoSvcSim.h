//
// Created by developer on 3/19/23.
//

#ifndef ALGO_SVC_AMPQ_POC_VIDEOSVCSIM_H
#define ALGO_SVC_AMPQ_POC_VIDEOSVCSIM_H

#include "cpptime.h"
#include "AmqpSub.h"
#include "AmqpPub.h"

class VideoSvcSim : public AMQPReceiveHandler
{
public:
    explicit VideoSvcSim(AmqpPub &amqpPub);
    ~VideoSvcSim();

    void Run();

private:
    char _videoOutBuff[1024 * 25]{};
    uint32_t _frameId{};
    AmqpPub &_amqpPub;

    CppTime::Timer _timer;
    unsigned long  _timer_30Hz{};

    void OnTimer_30Hz();

    void OnMsgReceived(const char *fromExchange, const char *fromRoute, const char *data, size_t dataSize,const char *correlationId,
                       uint64_t deliveryTag) override;

    void SendSimulatedEncodedFrame();
};


#endif //ALGO_SVC_AMPQ_POC_VIDEOSVCSIM_H
