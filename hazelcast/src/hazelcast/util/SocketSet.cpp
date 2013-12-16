//
// Created by sancar koyunlu on 16/12/13.
//

#include "SocketSet.h"

namespace hazelcast {
    namespace util {

        fd_set SocketSet::get_fd_set() const {
            fd_set fdSet;
            FD_ZERO(&fdSet);
            std::set<client::Socket>::iterator it;
            for (it = sockets.begin(); it != sockets.end(); it++) {
                FD_SET(it->getSocketId(), &fdSet);
            }
            return fdSet;
        }

        int SocketSet::getHighestSocketId() const {
            if (!sockets.empty())
                return (*(sockets.rbegin())).getSocketId();
            else
                return 0;
        }
    }
}