//
// Created by developer on 8/8/22.
//

#include <iostream>
#include "AmqpPub.h"

AmqpPub::AmqpPub(const std::string& conn, const std::vector<std::string>& exchanges) {
    auto opts = AmqpClient::Channel::OpenOpts::FromUri(conn);
    _channel = AmqpClient::Channel::Open(opts);

    for (const auto& exchange:exchanges) {
        _channel->DeclareExchange(exchange,AmqpClient::Channel::EXCHANGE_TYPE_TOPIC, false, true);
    }

}

void AmqpPub::Publish(const char *exchange, const char *route, const char *data, size_t dataSize, const char *correlationId) {
    const std::string body(data, dataSize);
    AmqpClient::BasicMessage::ptr_t message = AmqpClient::BasicMessage::Create(body);

    if(correlationId != nullptr)
        message->CorrelationId(correlationId);

    try {
        _channel->BasicPublish(exchange, route, message, false, false);
    } catch (std::runtime_error &ex) {
        std::cout << "AmqpPub::Publish error: " << ex.what() << std::endl;
    }

}
