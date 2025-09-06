# Gossip Protocol Implementation

This document describes the gossip protocol implementation added to Bitcoin Core.

## Overview

The gossip protocol allows nodes to propagate arbitrary messages across the Bitcoin network using a simple flooding algorithm with duplicate prevention. This can be useful for:

- Network health information sharing
- Protocol upgrade announcements  
- Statistical data dissemination
- Future feature coordination

## Message Structure

Gossip messages have the following structure:

- `hash`: 256-bit unique identifier (SHA256 hash of timestamp + topic + data)
- `timestamp`: 32-bit Unix timestamp when message was created
- `topic`: Variable-length string identifying message category (max 32 chars)
- `data`: Variable-length payload (max 1024 bytes)

## Network Protocol

### Message Type

- Message type: `"gossip"`
- Added to `NetMsgType` namespace in `protocol.h`

### Flood Protection

Each peer maintains:
- Rate limiter: Maximum 10 gossip messages per minute per peer
- Duplicate filter: Rolling bloom filter tracking recently seen message hashes
- Size limits: 1KB maximum message payload, 32 character maximum topic length

### Propagation

When a node receives a valid gossip message:
1. Validates message format and limits
2. Checks rate limiting for the sender
3. Checks if message was already seen (via hash)
4. If valid and new, records in seen filter
5. Forwards to all other connected peers (except sender)

## RPC Interface

### `sendgossip`

Sends a gossip message to the network.

**Parameters:**
- `topic` (string): Message category/topic
- `message` (string): Hex-encoded message content

**Returns:**
- `hash` (string): Message hash 
- `peers` (number): Number of peers message was sent to

**Example:**
```bash
bitcoin-cli sendgossip "network_status" "48656c6c6f20576f726c64"
```

## Implementation Files

- `src/gossip.h` - Message structure and constants
- `src/gossip.cpp` - Message validation and hashing
- `src/protocol.h` - Network message type definition
- `src/net_processing.cpp` - Message handling and forwarding logic
- `src/rpc/net.cpp` - RPC interface implementation
- `src/test/gossip_tests.cpp` - Unit tests

## Security Considerations

- Rate limiting prevents spam
- Message size limits prevent resource exhaustion
- No authentication - messages are public and can be spoofed
- Duplicate prevention avoids network loops
- Simple flooding may not scale to very large networks

## Future Enhancements

Potential improvements could include:
- Message authentication/signatures
- More sophisticated routing algorithms
- Topic-based filtering/subscriptions
- Persistence of important messages
- Integration with existing Bitcoin network topology optimization