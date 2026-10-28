#pragma once

#include <QtTest>
#include <cstdio>
#include <functional>
#include <set>
#include <string>
#include <vector>

namespace zinsser {

inline std::string toLowerAscii(const std::string &input) {
    std::string result = input;
    for (char &ch : result) {
        if (ch >= 'A' && ch <= 'Z') {
            ch = static_cast<char>(ch - 'A' + 'a');
        }
    }
    return result;
}

inline void verifyClutterFreeProse(const char *text) {
    const std::string lower = toLowerAscii(text ? text : "");
    QVERIFY2(!lower.empty(), "Zinsser prose violation: empty describe/it title");
    const std::vector<std::string> bannedWords = {
        "should", "properly", "correctly", "successfully", "works as expected", "in order to"
    };
    for (const std::string &banned : bannedWords) {
        const bool clean = (lower.find(banned) == std::string::npos);
        QVERIFY2(clean, ("Zinsser prose violation: banned filler '" + banned + "' in '" + lower + "'").c_str());
    }
}

struct SuiteContext {
    std::string activeSubject;
    std::set<std::string> uniqueClaims;
};

inline SuiteContext &currentSuite() {
    static SuiteContext ctx;
    return ctx;
}

template <typename Fn>
inline void it(const char *claim, Fn &&body) {
    verifyClutterFreeProse(claim);
    SuiteContext &ctx = currentSuite();
    QVERIFY2(!ctx.activeSubject.empty(), "it() must execute inside an active describe() block");
    const bool inserted = ctx.uniqueClaims.insert(claim).second;
    QVERIFY2(inserted, ("Duplicate it() claim in describe() block: " + std::string(claim)).c_str());
    std::printf("    + it %s\n", claim);
    body();
}

template <typename Fn>
inline void describe(const char *subject, Fn &&body) {
    verifyClutterFreeProse(subject);
    SuiteContext &ctx = currentSuite();
    ctx.activeSubject = subject;
    ctx.uniqueClaims.clear();
    std::printf("\n  [DESCRIBE] %s\n", subject);
    body();
    const size_t count = ctx.uniqueClaims.size();
    ctx.activeSubject.clear();
    QVERIFY2(count >= 4, ("describe('" + std::string(subject) + "') requires >= 4 unique it() blocks").c_str());
}

} // namespace zinsser

using zinsser::describe;
using zinsser::it;
