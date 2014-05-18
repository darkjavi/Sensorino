#include "Sensorino.h"
#include "Service.h"

Service::Service(int _id) {
    sensorino->addService(this);
}

void Service::handleMessage(Message *message) {
    switch (message->getMessageType()) {
    case SET:
        return onSet(message);

    case REQUEST:
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

Message *Service::*publish(Message *message) {
    /* NOTE: we may want to reference the original message Id in the
     * response somehow.
     */
    return startBaseMesssage(PUBLISH, message);
}

Message *Service::*err(Message *message) {
    /* NOTE: we may want to reference the original message Id in the
     * response somehow.
     */
    return startBaseMesssage(ERR, message);
}

Message *Service::startBaseMessage(MessageType type, Message *orig) {
    /* NOTE: internal messages (not transmitted over radio.. where
     * sender == addressee) may be implemented here or somewhere else.
     */
    /* REVISIT: it may be desirable to avoid the malloc by using one
     * (or a few) static Message instances over and over.
     */
    uint8_t src = sensorino->getAddress();
    uint8_t dst = sensorino->getBaseAddress();

    if (orig)
        dst = orig->getDstAddress();

    return new Message(src, dst);
}
/* vim: set sw=4 ts=4 et: */