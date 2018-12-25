#include "uuidpp.h"
#include <lace/hash.h>

#include <cassert>
#include <cstdlib>

#include <iostream>
#include <sstream>

#include <vector>

int main(int argc, char* argv[]) {
    for (int i = 0 ; i < argc ; ++i) {
        uuid u;
        std::stringstream(argv[i]) >> u;
        std::cout
            << (bool)u << '\t'
            << argv[i] << '\t'
            << u << '\t'
            << (u.temporal() ? u.time() : 0) << '\t'
            << lace::hash(u)
            << std::endl;
    }
    for(auto const & l : std::vector<const char *>{"@dns", "@url", "@oid", "@x500"}) {
        const uuid u(l);
        std::cout
            << u << '\t'
            << (u.temporal() ? u.time() : 0) << '\t'
            << lace::hash(u)
            << std::endl;
    }
    assert(uuid("@dns") == uuid("@dns"));
    assert(uuid("@dns") != uuid("@url"));
    assert(uuid("@dns") < uuid("@url"));
    assert(uuid("@url") > uuid("@dns"));

    assert(uuid("@dns") == uuid::dns);
    assert(uuid("@url") == uuid::url);
    assert(uuid("@oid") == uuid::oid);
    assert(uuid("@x500") == uuid::x500);
    {
        const uuid u(uuid::type_t::random);
        assert(!u.temporal());
        std::cout
            << u << '\t'
            << (u.temporal() ? u.time() : 0) << '\t'
            << lace::hash(u)
            << std::endl;
    }
    {
        const uuid u(uuid::type_t::time);
        assert(u.temporal());
        std::cout
            << u << '\t'
            << (u.temporal() ? u.time() : 0) << '\t'
            << lace::hash(u)
            << std::endl;
    }
    {
        const uuid u(uuid("@oid"), "oh hai");
        std::cout
            << u << '\t'
            << (u.temporal() ? u.time() : 0) << '\t'
            << lace::hash(u)
            << std::endl;
        assert(0 == uuid(true).sha1(uuid("@oid"), "oh hai").cmp(u));
        assert(u == uuid(true).sha1(uuid("@oid"), "oh hai"));
        assert(0 != uuid(true).md5(uuid("@oid"), "oh hai").cmp(u));
        assert(u != uuid(true).md5(uuid("@oid"), "oh hai"));
    }
    assert(lace::hash(uuid(uuid("@oid"), "oh hai")) == lace::hash(uuid(uuid("@oid"), "oh hai")));
    assert(lace::hash(uuid(uuid("@oid"), "oh hai")) != lace::hash(uuid(uuid("@oid"), "meh no")));
    return EXIT_SUCCESS;
}
