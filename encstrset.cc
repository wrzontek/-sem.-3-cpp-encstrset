#include "encstrset.h"
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#ifdef NDEBUG
constexpr static bool debug = false;
#else
constexpr static bool debug = true;
#endif

namespace {
    std::unordered_map<unsigned long, std::unordered_set<std::string>> &sets() {
        static auto *sets = new std::unordered_map<unsigned long, std::unordered_set<std::string>>;
        return *sets;
    }

    unsigned long current_set_id = 0;
}

#ifdef __cplusplus
namespace jnp1 {
    extern "C" {
#endif

    unsigned long encstrset_new() {
        sets().insert({current_set_id, *new std::unordered_set<std::string>});

        if (debug)
            std::cerr << "encstrset_new: set #" << current_set_id << " created\n";

        current_set_id++;
        return (current_set_id - 1);
    }

    void encstrset_delete(unsigned long id) {
        if (sets().count(id)) {
            sets().erase(id);
            if (debug)
                std::cerr << "encstrset_delete: set #" << id << " deleted\n";
        } else {
            if (debug)
                std::cerr << "encstrset_delete: set #" << id << " does not exist\n";
        }
    }

    size_t encstrset_size(unsigned long id) {}

    bool encstrset_insert(unsigned long id, const char *value, const char *key) {}

    bool encstrset_remove(unsigned long id, const char *value, const char *key) {}

    bool encstrset_test(unsigned long id, const char *value, const char *key) {}

    void encstrset_clear(unsigned long id) {}

    void encstrset_copy(unsigned long src_id, unsigned long dst_id) {}

#ifdef __cplusplus
    }
}
#endif

