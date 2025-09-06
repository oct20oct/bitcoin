// Copyright (c) 2025 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <gossip.h>

#include <hash.h>
#include <streams.h>

void CGossipMessage::UpdateHash()
{
    DataStream ss{};
    ss << timestamp << topic << data;
    hash = Hash(ss);
}

bool CGossipMessage::IsValid() const
{
    // Check size limits
    if (data.size() > MAX_GOSSIP_MESSAGE_SIZE) return false;
    if (topic.size() > 32) return false; // Reasonable topic length limit
    if (topic.empty()) return false;
    
    // Check timestamp is not too far in the future (allow 10 minute clock skew)
    uint32_t current_time = GetTime();
    if (timestamp > current_time + 600) return false;
    
    // Check hash is correct
    CGossipMessage temp_msg = *this;
    temp_msg.UpdateHash();
    return temp_msg.hash == hash;
}