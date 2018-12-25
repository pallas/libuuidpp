#include "uuidpp.h"

#include <cassert>
#include <stdexcept>

#include <cstring>
#include <iostream>

uuid::uuid() { clear(); }
uuid::uuid(bool) { }

uuid::uuid(const uuid & that) { uuid_copy(_, that._); }
uuid::uuid(const uuid_t that) { uuid_copy(_, that); }

uuid::uuid(const char * s) { if (!parse(s)) clear(); }
uuid::uuid(const std::string &s) { if (!parse(s.c_str())) clear(); }

uuid::uuid(const uuid & ns, const char * s) { sha1(ns,s); }
uuid::uuid(const uuid & ns, const char * s, size_t l) { sha1(ns,s,l); }
uuid::uuid(const uuid & ns, const std::string &s) { sha1(ns,s); }

uuid::uuid(int t) {
    switch (t) {
    case type_t::random:
        uuid_generate_random(_);
        break;
    case type_t::time:
        if (0 != uuid_generate_time_safe(_))
            clear();
        break;
    default:
        clear();
        return;
    }
    assert(t == type());
}

uuid uuid::make_time() { return uuid(type_t::time); }
uuid uuid::make_random() { return uuid(type_t::random); }

uuid::operator bool() const { return !uuid_is_null(_); }

uuid &
uuid::operator=(const uuid & that) {
    uuid_copy(_, that._);
    return *this;
}

bool
uuid::parse(const char * s) {
    if (!uuid_parse(s, _))
        return true;

    if (s && s[0] == '@')
    if (const uuid_t *u = uuid_get_template(s+1)) {
        uuid_copy(_, *u);
        return true;
    }

    return false;
}

bool uuid::parse(const std::string &s) { return parse(s.c_str()); }

std::string
uuid::unparse() const {
    std::string s(UUID_STR_LEN, 0);
    uuid_unparse(_, &s.front());
    s.resize(UUID_STR_LEN-1);
    return s;
}

lace::compare_t
uuid::cmp(const uuid &that) const {
    return uuid_compare(_, that._);
}

lace::hash_t
uuid::hc() const {
    return lace::detail::fnv_1a(reinterpret_cast<const unsigned char *>(&_), sizeof(_));
}

void uuid::clear() { uuid_clear(_); }

uuid & uuid::md5(const uuid &ns, const char *s) { return md5(ns, s, strlen(s)); }
uuid & uuid::md5(const uuid &ns, const std::string &s) { return md5(ns, s.data(), s.length()); }

uuid &
uuid::md5(const uuid &ns, const char *s, size_t l) {
    assert(ns);
    uuid_generate_md5(_, ns._, s, l);
    return *this;
}

uuid & uuid::sha1(const uuid &ns, const char *s) { return sha1(ns, s, strlen(s)); }
uuid & uuid::sha1(const uuid &ns, const std::string &s) { return sha1(ns, s.data(), s.length()); }

uuid &
uuid::sha1(const uuid &ns, const char *s, size_t l) {
    assert(ns);
    uuid_generate_sha1(_, ns._, s, l);
    return *this;
}

bool uuid::temporal() const { return type_t::time == type(); }

time_t
uuid::time() const {
    assert(temporal());
    return uuid_time(_, NULL);
}

struct timeval
uuid::timeval() const {
    assert(temporal());
    struct timeval t;
    uuid_time(_, &t);
    return t;
}

int uuid::type() const { return uuid_type(_); }
int uuid::variant() const { return uuid_variant(_); }

std::ostream &
operator<< (std::ostream &os, const uuid &u) {
    return os << u.unparse();
}

std::istream & operator>> (std::istream &is, uuid &u) {
    std::string s;
    s.reserve(UUID_STR_LEN);
    if (!(is >> s && u.parse(s)))
        u.clear();
    return is;
}

const uuid uuid::dns("@dns");
const uuid uuid::url("@url");
const uuid uuid::oid("@oid");
const uuid uuid::x500("@x500");

//
