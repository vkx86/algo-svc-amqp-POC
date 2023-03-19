//
// Created by developer on 3/19/23.
//

#ifndef ALGO_SVC_AMPQ_POC_COMMONDEFS_H
#define ALGO_SVC_AMPQ_POC_COMMONDEFS_H

#define AMQP_CONN_STR "amqp://guest:guest@localhost"

#define EXCH_VIDEO_IN "exchange.topic.video.in"
#define ROUTE_VIDEO_IN "route.video.in"
#define EXCH_VIDEO_OUT "exchange.topic.video.out"
#define ROUTE_VIDEO_OUT "route.video.out"

struct VideoFrameMsg
{
    uint32_t FrameId;
    uint32_t FrameLength;
    timespec SentTime;
    uint8_t Body[0];
}__attribute__((packed));


#endif //ALGO_SVC_AMPQ_POC_COMMONDEFS_H
