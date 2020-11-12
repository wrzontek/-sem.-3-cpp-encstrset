#include "encstrset.h"
#include <unordered_map>
#include <unordered_set>
#include <iomanip>
#include <cstring>

#ifdef NDEBUG
constexpr static bool debug = false;
#else
constexpr static bool debug = true;
#endif

namespace {
    using map_of_string_sets_t = std::unordered_map<unsigned long, std::unordered_set<std::string>>;

    map_of_string_sets_t &sets() {
        static auto *sets = new map_of_string_sets_t;
        return *sets;
    }

    unsigned long &current_set_id() {
        static unsigned long current_set_id = 0;
        return current_set_id;
    }

    std::string symmetrical_enc_dec(const char *value, const char *key) {
        std::string result(value);
        if (key != nullptr) {
            size_t key_length = strlen(key);
            if (key_length > 0) {
                size_t i = 0;
                for (auto& value: result) {
                    value ^= key[(i++) % key_length];
                }
            }
        }

        return result;
    }

    void cerr_print_cypher_hex(const std::string &cypher) {
        std::cerr << "cypher "<< "\"";
        for (size_t i = 0; i < cypher.size(); ++i) {
            std::cerr << ((i > 0) ? " ": "");
            std::cerr << std::setfill('0') << std::setw(2)
                      << std::hex << std::uppercase << static_cast<int>(cypher[i]);
        }
        std::cerr << "\"";
    }

    void cerr_print_statement(unsigned long id, const std::string &cerr_command_name,
                              const std::string &cypher, const std::string &ending) {
        std::cerr << cerr_command_name << ": set #" << id << ", ";
        cerr_print_cypher_hex(cypher);
        std::cerr << ending << "\n";
    }

    void cerr_print_intro(unsigned long id, const std::string &cerr_command_name,
                          const char *value, const char *key) {
        std::cerr << cerr_command_name;
        std::cerr << "(" << id << ", ";

        if (value == nullptr) {
            const std::string null_value = "NULL";
            std::cerr << null_value << ", ";
        } else {
            std::cerr << "\"" << value << "\", ";
        }

        if (key == nullptr) {
            const std::string null_key = "NULL";
            std::cerr << null_key << ")\n";
        } else {
            std::cerr << "\"" << key << "\")\n";
        }
    }

    void set_not_exists(unsigned long id, const std::string &cerr_command_name) {
        std::cerr << cerr_command_name << ": set #"<< id << " does not exist\n";
    }

    void invalid_value(const std::string &cerr_command_name) {
        std::cerr << cerr_command_name << ": invalid value (NULL)\n";
    }

    bool first_parameter_null(const char *value) {
        return (value == nullptr);
    }

    bool is_cypher_present(map_of_string_sets_t::const_iterator set_iter,
                           const std::string &cypher) {

        auto cypher_iter = set_iter->second.find(cypher);
        return (cypher_iter != set_iter->second.end());

    }

}


namespace jnp1 {

    unsigned long encstrset_new() {
        static const std::string cerr_command_name = __func__;

        if (debug)
            std::cerr << cerr_command_name << "()\n";

        sets().insert({current_set_id(), *new std::unordered_set<std::string>});

        if (debug)
            std::cerr << cerr_command_name << ": set #" << current_set_id() << " created\n";

        ++current_set_id();
        return (current_set_id() - 1);
    }

    void encstrset_delete(unsigned long id) {
        static const std::string cerr_command_name = __func__;

        if (debug)
            std::cerr << cerr_command_name << "(" << id << ")\n";

        auto set_iter = sets().find(id);

        if (set_iter != sets().end()) {
            sets().erase(set_iter);

            if (debug)
                std::cerr << cerr_command_name << ": set #" << id << " deleted\n";

        } else if (debug)
            set_not_exists(id, cerr_command_name);
    }

    size_t encstrset_size(unsigned long id) {
        static const std::string cerr_command_name = __func__;

        if (debug)
            std::cerr << cerr_command_name << "(" << id << ")\n";

        auto set_iter = sets().find(id);

        if (set_iter != sets().end()) {
            size_t size = set_iter->second.size();

            if (debug)
                std::cerr << cerr_command_name << ": set #" << id
                          << " contains " << size << " element(s)\n";

            return size;

        } else {
            if (debug)
                set_not_exists(id, cerr_command_name);
            return 0;
        }
    }

    bool encstrset_insert(unsigned long id, const char *value, const char *key) {
        static const std::string cerr_command_name = __func__;

        if (debug)
            cerr_print_intro(id, cerr_command_name, value, key);

        if (!first_parameter_null(value)) {
            auto set_iter = sets().find(id);
            if (set_iter != sets().end()) {
                std::string cypher = symmetrical_enc_dec(value, key);
                if (!is_cypher_present(set_iter, cypher)) {

                    if (debug)
                        cerr_print_statement(id, cerr_command_name, cypher, " inserted");

                    set_iter->second.insert(cypher);
                    return true;

                } else if (debug)
                    cerr_print_statement(id, cerr_command_name, cypher, " was already present");

            } else if (debug)
                set_not_exists(id, cerr_command_name);

        } else if (debug)
            invalid_value(cerr_command_name);

        return false;
    }

    bool encstrset_remove(unsigned long id, const char *value, const char *key) {
        static const std::string cerr_command_name = __func__;
        if (debug)
            cerr_print_intro(id, cerr_command_name, value, key);

        if (!first_parameter_null(value)) {

            auto set_iter = sets().find(id);
            if (set_iter != sets().end()) {
                std::string cypher = symmetrical_enc_dec(value, key);
                if (is_cypher_present(set_iter, cypher)) {
                    set_iter->second.erase(cypher);

                    if (debug)
                        cerr_print_statement(id, cerr_command_name, cypher, " removed");

                    return true;
                } else if (debug)
                    cerr_print_statement(id, cerr_command_name, cypher, " was not present");

            } else if (debug)
                set_not_exists(id, cerr_command_name);

        } else if (debug)
            invalid_value(cerr_command_name);

        return false;
    }

    bool encstrset_test(unsigned long id, const char *value, const char *key) {
        static const std::string cerr_command_name = __func__;
        if (debug)
            cerr_print_intro(id, cerr_command_name, value, key);

        if (!first_parameter_null(value)) {

            auto set_iter = sets().find(id);
            if (set_iter != sets().end()) {
                std::string cypher = symmetrical_enc_dec(value, key);
                if (is_cypher_present(set_iter, cypher)) {
                    if (debug)
                        cerr_print_statement(id, cerr_command_name, cypher, " is present");

                    return true;
                } else if (debug)
                    cerr_print_statement(id, cerr_command_name, cypher, " is not present");

            } else if (debug)
                set_not_exists(id, cerr_command_name);

        } else if (debug)
            invalid_value(cerr_command_name);

        return false;
    }

    void encstrset_clear(unsigned long id) {
        static const std::string cerr_command_name = __func__;

        if (debug)
            std::cerr << cerr_command_name << "(" << id << ")\n";

        if (sets().find(id) != sets().end()) {
            sets()[id].clear();

            if (debug)
                std::cerr << cerr_command_name << ": set #" << id << " cleared\n";

        } else if (debug)
            set_not_exists(id, cerr_command_name);
    }

    void encstrset_copy(unsigned long src_id, unsigned long dst_id) {
        static const std::string cerr_command_name = __func__;
        if (debug)
            std::cerr << cerr_command_name << "(" << src_id << ", " << dst_id << ")\n";

        auto src_iter = sets().find(src_id);
        auto dst_iter = sets().find(dst_id);
        bool src_exists = (src_iter != sets().end());
        bool dst_exists = (dst_iter != sets().end());

        if (src_exists && dst_exists) {

            for (auto cypher: src_iter->second) {

                if (debug) {
                    auto it = dst_iter->second.find(cypher);
                    std::cerr << cerr_command_name << ": ";
                    if (it != dst_iter->second.end()) {
                        std::cerr << "copied ";
                        cerr_print_cypher_hex(cypher);
                        std::cerr << " was already present in set #" << dst_id << "\n";
                    } else {
                        cerr_print_cypher_hex(cypher);
                        std::cerr << " copied from set #" << src_id << " to set #" << dst_id << "\n";
                    }
                }

                dst_iter->second.insert(cypher);
            }

        } else if (debug) {
            if (!src_exists)
                set_not_exists(src_id, cerr_command_name);
            else
                set_not_exists(dst_id, cerr_command_name);
        }
    }
}

