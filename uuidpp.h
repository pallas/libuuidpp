#ifndef LACE__UUIDPP_H
#define LACE__UUIDPP_H

#include <uuid/uuid.h>
#include <lace/compare.h>
#include <lace/hash.h>
#include <sys/time.h>

#include <string>
#include <iosfwd>

class uuid {
public:
    uuid();
    uuid(bool);

    uuid(const uuid &);
    uuid(const uuid_t);

    uuid(const char *);
    uuid(const std::string &);

    uuid(const uuid &, const char *);
    uuid(const uuid &, const char *, size_t);
    uuid(const uuid &, const std::string &);

    uuid(int);

    static uuid make_time();
    static uuid make_random();

    operator bool() const;
    uuid & operator=(const uuid &);

    bool parse(const char *);
    bool parse(const std::string &);
    std::string unparse() const;
    lace::compare_t cmp(const uuid &) const;
    lace::hash_t hc() const;

    void clear();

    uuid & md5(const uuid &, const char *);
    uuid & md5(const uuid &, const char *, size_t);
    uuid & md5(const uuid &, const std::string &);

    uuid & sha1(const uuid &, const char *);
    uuid & sha1(const uuid &, const char *, size_t);
    uuid & sha1(const uuid &, const std::string &);

    bool temporal() const;
    time_t time() const;
    struct timeval timeval() const;

    int type() const;
    struct type_t { enum {
        time = UUID_TYPE_DCE_TIME,
        security = UUID_TYPE_DCE_SECURITY,
        md5 = UUID_TYPE_DCE_MD5,
        random = UUID_TYPE_DCE_RANDOM,
        sha1 = UUID_TYPE_DCE_SHA1,
    }; };

    int variant() const;
    struct variant_t { enum {
        ncs = UUID_VARIANT_NCS,
        dce = UUID_VARIANT_DCE,
        microsoft = UUID_VARIANT_MICROSOFT,
        other = UUID_VARIANT_OTHER,
    }; };

    friend std::ostream & operator<< (std::ostream &, const uuid &);
    friend std::istream & operator>> (std::istream &, uuid &);

    static const uuid dns;
    static const uuid url;
    static const uuid oid;
    static const uuid x500;
private:
    uuid_t _;
};

LACE_COMPARABLE(uuid, cmp);
LACE_HASHABLE(uuid, hc);

#endif//LACE__UUIDPP_H
