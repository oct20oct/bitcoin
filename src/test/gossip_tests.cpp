// Copyright (c) 2025 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <gossip.h>

#include <test/util/setup_common.h>
#include <util/time.h>

#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(gossip_tests, TestingSetup)

BOOST_AUTO_TEST_CASE(gossip_message_creation)
{
    std::string topic = "test_topic";
    std::vector<unsigned char> data = {0x01, 0x02, 0x03, 0x04};
    
    CGossipMessage msg(topic, data);
    
    BOOST_CHECK_EQUAL(msg.topic, topic);
    BOOST_CHECK(msg.data == data);
    BOOST_CHECK(msg.timestamp > 0);
    BOOST_CHECK(!msg.hash.IsNull());
    BOOST_CHECK(msg.IsValid());
}

BOOST_AUTO_TEST_CASE(gossip_message_validation)
{
    // Valid message
    CGossipMessage valid_msg("valid_topic", {0x01, 0x02});
    BOOST_CHECK(valid_msg.IsValid());
    
    // Invalid: empty topic
    CGossipMessage empty_topic_msg("", {0x01, 0x02});
    BOOST_CHECK(!empty_topic_msg.IsValid());
    
    // Invalid: too large topic
    std::string long_topic(50, 'a'); // 50 characters, should be invalid
    CGossipMessage long_topic_msg(long_topic, {0x01, 0x02});
    BOOST_CHECK(!long_topic_msg.IsValid());
    
    // Invalid: too large data
    std::vector<unsigned char> large_data(MAX_GOSSIP_MESSAGE_SIZE + 1, 0x01);
    CGossipMessage large_data_msg("test", large_data);
    BOOST_CHECK(!large_data_msg.IsValid());
}

BOOST_AUTO_TEST_CASE(gossip_message_hash_consistency)
{
    std::string topic = "hash_test";
    std::vector<unsigned char> data = {0xaa, 0xbb, 0xcc};
    
    CGossipMessage msg1(topic, data);
    CGossipMessage msg2(topic, data);
    
    // Messages with same content but created at different times should have different hashes
    // due to timestamp difference
    BOOST_CHECK(msg1.hash != msg2.hash);
    
    // Same message should validate correctly
    uint256 original_hash = msg1.hash;
    BOOST_CHECK(msg1.IsValid());
    BOOST_CHECK_EQUAL(msg1.hash, original_hash); // Hash shouldn't change during validation
}

BOOST_AUTO_TEST_SUITE_END()