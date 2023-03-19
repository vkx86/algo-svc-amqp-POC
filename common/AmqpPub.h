//
// Created by developer on 8/8/22.
//

#ifndef DAG_AIR_AMQPPUB_H
#define DAG_AIR_AMQPPUB_H

#include <string>
#include <SimpleAmqpClient/Channel.h>

class AmqpPub {
public:
    AmqpPub(const std::string& conn, const std::vector<std::string>& exchanges);

    void Publish(const char *exchange, const char *route, const char *data, size_t dataSize, const char *correlationId = nullptr);
private:
    AmqpClient::Channel::ptr_t _channel;
};


#endif //DAG_AIR_AMQPPUB_H
