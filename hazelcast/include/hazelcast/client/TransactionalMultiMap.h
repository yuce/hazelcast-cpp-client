//
// Created by sancar koyunlu on 8/6/13.
// Copyright (c) 2013 hazelcast. All rights reserved.

#ifndef HAZELCAST_TransactionalMultiMap
#define HAZELCAST_TransactionalMultiMap

#include "Data.h"
#include "TxnMultiMapPutRequest.h"
#include "TxnMultiMapGetRequest.h"
#include "PortableCollection.h"
#include "TxnMultiMapRemoveRequest.h"
#include "TxnMultiMapValueCountRequest.h"
#include "TxnMultiMapSizeRequest.h"
#include "TransactionProxy.h"
#include "MultiMapDestroyRequest.h"

namespace hazelcast {
    namespace client {

        template<typename K, typename V>
        class TransactionalMultiMap {
            friend class TransactionContext;

        public:

            bool put(const K& key, const V& value) {
                serialization::Data keyData = toData(key);
                serialization::Data valueData = toData(value);
                multimap::TxnMultiMapPutRequest request(name, keyData, valueData);
                return invoke<bool>(request);
            };

            std::vector<V> get(const K& key) {
                serialization::Data data = toData(key);
                multimap::TxnMultiMapGetRequest request(name, data);
                impl::PortableCollection portableCollection = invoke<impl::PortableCollection>(request);
                vector<serialization::Data> const & dataCollection = portableCollection.getCollection();
                vector<serialization::Data>::iterator it;
                std::vector<V> result;
                result.resize(dataCollection.size());
                for (int i = 0; i < dataCollection.size(); i++) {
                    result[i] = toObject<V>(dataCollection[i]);
                }
                return result;
            };

            bool remove(const K& key, const V& value) {
                serialization::Data dataKey = toData(key);
                serialization::Data dataValue = toData(value);
                multimap::TxnMultiMapRemoveRequest request(name, dataKey, dataValue);
                return invoke<bool>(request);
            };

            std::vector<V> remove(const K& key) {
                serialization::Data data = toData(key);
                multimap::TxnMultiMapRemoveRequest request(name, &data);
                impl::PortableCollection portableCollection = invoke<impl::PortableCollection>(request);
                vector<serialization::Data> const & dataCollection = portableCollection.getCollection();
                vector<serialization::Data>::iterator it;
                std::vector<V> result;
                result.resize(dataCollection.size());
                for (int i = 0; i < dataCollection.size(); i++) {
                    result[i] = toData(&(dataCollection[i]));
                }
                return result;
            };


            int valueCount(const K& key) {
                serialization::Data data = toData(key);
                multimap::TxnMultiMapValueCountRequest request(name, data);
                return invoke<int>(request);
            }

            int size() {
                multimap::TxnMultiMapSizeRequest request(name);
                return invoke<int>(request);
            }

            std::string getName() const {
                return name;
            }

            void destroy() {
                multimap::MultiMapDestroyRequest request (name);
                invoke<bool>(request);
            }

        private :

            txn::TransactionProxy *transaction;
            std::string name;

            void init(const std::string& name, txn::TransactionProxy *transactionProxy) {
                this->transaction = transactionProxy;
                this->name = name;
            };

            template<typename T>
            serialization::Data toData(const T& object) {
                return transaction->getSerializationService().toData<T>(&object);
            };

            template<typename T>
            T toObject(const serialization::Data& data) {
                return transaction->getSerializationService().template toObject<T>(data);
            };

            template<typename Response, typename Request>
            Response invoke(const Request& request) {
                return transaction->sendAndReceive<Response>(request);
            };

        };
    }
}


#endif //HAZELCAST_TransactionalMultiMap
