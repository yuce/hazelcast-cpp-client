//
// Created by sancar koyunlu on 6/25/13.
// Copyright (c) 2013 hazelcast. All rights reserved.



#ifndef HAZELCAST_RemoveAllRequest
#define HAZELCAST_RemoveAllRequest

#include "hazelcast/client/multimap/KeyBasedRequest.h"

namespace hazelcast {
    namespace client {
        namespace multimap {
            class RemoveAllRequest : public KeyBasedRequest {
            public:
                RemoveAllRequest(const std::string& name, const serialization::Data& key, int threadId);

                int getClassId() const;

                void writePortable(serialization::PortableWriter& writer) const;

            private:
                int threadId;
            };
        }
    }
}

#endif //HAZELCAST_RemoveAllRequest