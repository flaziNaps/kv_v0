#include <gtest/gtest.h>
#include <kv/core/memory_store.hpp>

class MemoryStoreTest : public testing::Test
{
protected:
    kv::MemoryStore store;
};

TEST_F(MemoryStoreTest, SetThenGet)
{
    store.set("a", "1");
    EXPECT_EQ(store.get("a"), std::optional<std::string>{"1"});
}

TEST_F(MemoryStoreTest, MissingKeyReturnsNullopt)
{
    EXPECT_EQ(store.get("nope"), std::nullopt);
}

TEST_F(MemoryStoreTest, SetOverwritesExistingValue)
{
    store.set("a", "1");
    store.set("a", "2");
    EXPECT_EQ(store.get("a"), std::optional<std::string>{"2"});
}

TEST_F(MemoryStoreTest, DeleteRemovesKey)
{
    store.set("a", "1");
    EXPECT_TRUE(store.del("a"));
    EXPECT_EQ(store.get("a"), std::nullopt);
    EXPECT_FALSE(store.contains("a"));
}

TEST_F(MemoryStoreTest, DeleteMissingKeyReturnsFalse)
{
    EXPECT_FALSE(store.del("nope"));
}

TEST_F(MemoryStoreTest, DeleteTwiceReturnsFalse)
{
    store.set("a", "1");
    EXPECT_TRUE(store.del("a"));
    EXPECT_FALSE(store.del("a"));
}

TEST_F(MemoryStoreTest, ContainsReflectsPresence)
{
    store.set("a", "1");
    EXPECT_TRUE(store.contains("a"));
    EXPECT_FALSE(store.contains("nope"));
}
