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
        if (debug)
            std::cerr << "encstrset_new()\n";

        sets().insert({current_set_id, *new std::unordered_set<std::string>});

        if (debug)
            std::cerr << "encstrset_new: set #" << current_set_id << " created\n";

        current_set_id++;
        return (current_set_id - 1);
    }

    void encstrset_delete(unsigned long id) {
        if (debug)
            std::cerr << "encstrset_delete(" << id << ")\n";

        if (sets().count(id)) {
            sets().erase(id);
            if (debug)
                std::cerr << "encstrset_delete: set #" << id << " deleted\n";
        } else if (debug)
                std::cerr << "encstrset_delete: set #" << id << " does not exist\n";
    }

    size_t encstrset_size(unsigned long id) {
        if (debug)
            std::cerr << "encstrset_size(" << id << ")\n";

        if (sets().count(id)) {
            size_t size = sets()[id].size();

            if (debug)
                std::cerr << "encstrset_size: set #" << id << " contains " << size << " element(s)\n";

            return size;

        } else {
            if (debug)
                std::cerr << "encstrset_size: set #" << id << " does not exist\n";
            return 0;
        }
    }

    bool encstrset_insert(unsigned long id, const char *value, const char *key) {}

    bool encstrset_remove(unsigned long id, const char *value, const char *key) {}

    bool encstrset_test(unsigned long id, const char *value, const char *key) {}

    void encstrset_clear(unsigned long id) {
        if (debug)
            std::cerr << "encstrset_clear(" << id << ")\n";

        if (sets().count(id)) {
            sets()[id].clear();

            if (debug)
                std::cerr << "encstrset_size: set #" << id << " cleared\n";

        } else if (debug)
                std::cerr << "encstrset_size: set #" << id << " does not exist\n";
    }

    void encstrset_copy(unsigned long src_id, unsigned long dst_id) {
        if (debug)
            std::cerr << "encstrset_copy(" << src_id << ", " << dst_id << ")\n";

        bool src_exists = sets().count(src_id);
        bool dst_exists = sets().count(dst_id);
        if (src_exists && dst_exists) {

            // TODO

        } else if (debug) {
            if (!src_exists)
                std::cerr << "encstrset_size: set #" << src_id << " does not exist\n";
            if (!dst_exists)
                std::cerr << "encstrset_size: set #" << dst_id << " does not exist\n";
        }
    }

#ifdef __cplusplus
    }
}
#endif

