//
// Created by sancar koyunlu on 9/2/13.
// Copyright (c) 2013 hazelcast. All rights reserved.


#include "QueryResultSet.h"
#include "hazelcast/client/map/DataSerializableHook.h"
#include "ObjectDataOutput.h"
#include "ObjectDataInput.h"


namespace hazelcast {
    namespace client {
        namespace impl {
            QueryResultSet::QueryResultSet():data(true) {

            };

            QueryResultSet::QueryResultSet(const std::string& name, const std::string& iterationType, const std::string& sql)
            :iterationType(iterationType)
            , data(true) {

            };

            int QueryResultSet::getFactoryId() const {
                return map::DataSerializableHook::F_ID;
            }

            int QueryResultSet::getClassId() const {
                return map::DataSerializableHook::QUERY_RESULT_SET;
            };

            const std::vector<QueryResultEntry>& QueryResultSet::getResultData() const {
                return q;
            };

            void QueryResultSet::writeData(serialization::ObjectDataOutput& out) const {
                out.writeBoolean(data);
                out.writeUTF(iterationType);
                out.writeInt(q.size());
                for (int i = 0; i < q.size(); i++) {
                    out.writeObject<QueryResultEntry>(&(q[i]));
                }
            };

            void QueryResultSet::readData(serialization::ObjectDataInput& in) {
                data = in.readBoolean();
                iterationType = in.readUTF();
                int size = in.readInt();
                q.resize(size);
                for (int i = 0; i < size; i++) {
                    q[i] = in.readObject<QueryResultEntry>();
                }

            };
        }
    }
}