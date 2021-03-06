#include "Sensorino.h"
#include "Service.h"

Service::Service(int _id) : id(_id) {
    sensorino->addService(this);
}

void Service::handleMessage(Message *message) {
    switch (message->getType()) {
    case Message::SET:
        return onSet(message);

    case Message::REQUEST:
        /* NOTE: some things, like the current state request or
         * description request may be implemented here so as to
         * remove this burden from Service implementers.
         */
        return onRequest(message);

    default:
        err(message)->send();
        return;
    }
}

Message *Service::publish(Message *message) {
    /* NOTE: we may want to reference the original message Id in the
     * response somehow.
     */
    return startBaseMessage(Message::PUBLISH, message);
}

Message *Service::err(Message *message, Data::Type type) {
    /* NOTE: we may want to reference the original message Id in the
     * response somehow.
     */
    Message *m = startBaseMessage(Message::ERR, message);

    if (type != (Data::Type) -1)
        m->addDataTypeValue(type);

    return m;
}

Message *Service::startBaseMessage(Message::Type type, Message *orig) {
    /* NOTE: internal messages (not transmitted over radio.. where
     * sender == addressee) may be implemented here or somewhere else.
     */
    /* REVISIT: it may be desirable to avoid the malloc by using one
     * (or a few) static Message instances over and over.
     */
    uint8_t src = sensorino->getAddress();
    uint8_t dst = sensorino->getBaseAddress();
    Message *msg;

    if (orig)
        dst = orig->getSrcAddress();

    msg = new Message(src, dst);
    msg->setType(type);
    msg->addIntValue(Data::SERVICE_ID, id);
    return msg;
}
/* vim: set sw=4 ts=4 et: */
