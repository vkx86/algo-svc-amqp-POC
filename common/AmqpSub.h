//
// Created by developer on 7/26/22.
//

#ifndef DAG_AIR_AMQPSUB_H
#define DAG_AIR_AMQPSUB_H

#include <SimpleAmqpClient/SimpleAmqpClient.h>

struct AMQPReceiveHandler {
    virtual void
    OnMsgReceived(const char *fromExchange, const char *fromRoute, const char *data, size_t dataSize, const char *correlationId,
                  uint64_t deliveryTag) = 0;
};

class AmqpSub {
public:
    AmqpSub(const std::string& conn, const std::vector<std::pair<const char*, const char*>>& exchanges_N_routes,
            const std::string& sweeperQueue, AMQPReceiveHandler *handler);
    ~AmqpSub();

    void Start();
    void Stop();

private:
    AmqpClient::Channel::ptr_t _channel;
    std::string _consumer;
    std::vector<std::string> _queues;
    AMQPReceiveHandler *_handler;

    bool _isRunning{};

    void FetchMessages();

    static std::string GetExchangeType(const std::string &exchangeName);

};


#endif //DAG_AIR_AMQPSUB_H
