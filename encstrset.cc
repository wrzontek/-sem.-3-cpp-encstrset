#include "encstrset.h"
#include <iostream>
#include <unordered_map>
#include <unordered_set>

std::unordered_map<unsigned long, std::unordered_set<std::string>>& sets() {
    static auto* sets = new std::unordered_map<unsigned long, std::unordered_set<std::string>>;
    return *sets;
}


#ifdef __cplusplus
namespace jnp1 {
    extern "C" {
#endif

    unsigned long encstrset_new() {
        
    }

    void encstrset_delete(unsigned long id) {}

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

