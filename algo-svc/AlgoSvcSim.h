//
// Created by developer on 3/19/23.
//

#ifndef ALGO_SVC_AMPQ_POC_ALGOSVCSIM_H
#define ALGO_SVC_AMPQ_POC_ALGOSVCSIM_H


#include "AmqpSub.h"
#include "AmqpPub.h"

class AlgoSvcSim : public AMQPReceiveHandler
{
public:
    explicit AlgoSvcSim(AmqpPub &amqpPub);

    void Run();

    void OnMsgReceived(const char *fromExchange, const char *fromRoute, const char *data, size_t dataSize,
                       const char *correlationId, uint64_t deliveryTag) override;

private:
//    char _videoOutBuff[1280 * 720 * 3]{};
    char _videoOutBuff[1024 * 25]{};

    AmqpPub &_amqpPub;

    void SendSimulatedDecodedFrame(uint32_t frameId);
};


#endif //ALGO_SVC_AMPQ_POC_ALGOSVCSIM_H
