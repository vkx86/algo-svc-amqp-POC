//
// Created by developer on 7/26/22.
//

#include <iostream>
#include "AmqpSub.h"

using namespace std;

AmqpSub::AmqpSub(const std::string& conn, const std::vector<std::pair<const char*, const char*>>& exchanges_N_routes, const std::string& sweeperQueue, AMQPReceiveHandler *handler) {

    auto opts = AmqpClient::Channel::OpenOpts::FromUri(conn);
    _channel = AmqpClient::Channel::Open(opts);

    auto queue = _channel->DeclareQueue(sweeperQueue);
    for (const auto& current:exchanges_N_routes) {
        _channel->DeclareExchange(current.first, GetExchangeType(current.first), false, true);
        _channel->BindQueue(queue, current.first, current.second);
    }
    _consumer = _channel->BasicConsume(queue);

    _handler = handler;
    _isRunning = true;
}

AmqpSub::~AmqpSub() {
    Stop();
}

void AmqpSub::FetchMessages() {

    AmqpClient::Envelope::ptr_t delivered;
    auto ret = _channel->BasicConsumeMessage(_consumer, delivered, 10);

    if(!ret)
        return;

    //vk:
    //delivered->Message()->ReplyTo().c_str() is always empty in BIT messages, WTF...

    _handler->OnMsgReceived(delivered->Exchange().c_str(), delivered->RoutingKey().c_str(),
                            delivered->Message()->Body().c_str(), delivered->Message()->Body().size(),
                            delivered->Message()->CorrelationId().c_str(), delivered->DeliveryTag());
}

void AmqpSub::Start() {
    while (_isRunning)
        FetchMessages();
}

void AmqpSub::Stop() {
    _isRunning = false;
}

std::string AmqpSub::GetExchangeType(const string &exchangeName) {

    if (exchangeName.find(".topic.") != std::string::npos) {
        return AmqpClient::Channel::EXCHANGE_TYPE_TOPIC;
    }

    if (exchangeName.find(".fanout.") != std::string::npos) {
        return AmqpClient::Channel::EXCHANGE_TYPE_FANOUT;
    }

    throw std::runtime_error("Exchange type if invalid!");
}
