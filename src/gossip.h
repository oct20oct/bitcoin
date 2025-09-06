// Copyright (c) 2025 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_GOSSIP_H
#define BITCOIN_GOSSIP_H

#include <serialize.h>
#include <uint256.h>
#include <util/time.h>

#include <string>
#include <vector>

/** Maximum size of a gossip message payload */
static constexpr unsigned int MAX_GOSSIP_MESSAGE_SIZE = 1024;

/** Maximum number of gossip messages to relay per minute per peer */
static constexpr unsigned int MAX_GOSSIP_RATE_PER_MINUTE = 10;

/** How long to remember gossip message hashes to prevent rebroadcast */
static constexpr auto GOSSIP_CACHE_TIME = 10min;

/**
 * Gossip message structure
 */
class CGossipMessage
{
public:
    uint256 hash;              //!< Unique identifier for this message
    uint32_t timestamp;        //!< When the message was created (Unix time)
    std::string topic;         //!< Message topic/category (limited length)
    std::vector<unsigned char> data; //!< Message payload

    CGossipMessage() = default;

    CGossipMessage(const std::string& topic_in, const std::vector<unsigned char>& data_in)
        : timestamp(GetTime()), topic(topic_in), data(data_in)
    {
        UpdateHash();
    }

    void UpdateHash();

    SERIALIZE_METHODS(CGossipMessage, obj)
    {
        READWRITE(obj.hash, obj.timestamp, obj.topic, obj.data);
    }

    bool IsValid() const;
};

#endif // BITCOIN_GOSSIP_H