#ifndef SRC_NODE_SNAP7_HELPERS_H_
#define SRC_NODE_SNAP7_HELPERS_H_

#include <napi.h>
#include <sstream>
#include <string>

namespace node_snap7 {

inline std::string ValueTypeName(const Napi::Value& value) {
    switch (value.Type()) {
    case napi_undefined:
        return "undefined";
    case napi_null:
        return "null";
    case napi_boolean:
        return "boolean";
    case napi_number:
        return "number";
    case napi_string:
        return "string";
    case napi_symbol:
        return "symbol";
    case napi_object:
        return "object";
    case napi_function:
        return "function";
    case napi_external:
        return "external";
    case napi_bigint:
        return "bigint";
    default:
        return "unknown";
    }
}

inline void ThrowArgCountError(Napi::Env env, size_t expected, size_t actual) {
    std::ostringstream msg;
    msg << "Expected at least " << expected << " argument";
    if (expected != 1) msg << "s";
    msg << ", got " << actual;
    Napi::TypeError::New(env, msg.str()).ThrowAsJavaScriptException();
}

inline void ThrowArgTypeError(Napi::Env env,
                              const Napi::CallbackInfo& info,
                              size_t index,
                              const char* expected) {
    std::string actual = (info.Length() > index) ? ValueTypeName(info[index]) : "missing";
    std::ostringstream msg;
    msg << "Argument " << (index + 1) << " must be " << expected << ", got " << actual;
    Napi::TypeError::New(env, msg.str()).ThrowAsJavaScriptException();
}

inline std::string ExpectedNameFrom(const char* methodName) {
    std::string name(methodName);
    if (name.rfind("Is", 0) == 0 && name.size() > 2) {
        name = name.substr(2);
    }
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return name;
}

inline std::string JoinExpected(const std::initializer_list<const char*>& methods) {
    std::ostringstream os;
    bool first = true;
    for (const char* m : methods) {
        if (!first) {
            os << " or ";
        }
        os << ExpectedNameFrom(m);
        first = false;
    }
    return os.str();
}

#define REQUIRE_MIN_ARGS(env, info, n)                                                             \
    if ((info).Length() < (n)) {                                                                   \
        ThrowArgCountError((env), (n), (info).Length());                                           \
        return (env).Undefined();                                                                  \
    }

#define REQUIRE_ARG(env, info, idx, method)                                                        \
    if ((info).Length() <= (idx) || !(info)[(idx)].method()) {                                     \
        ThrowArgTypeError((env), (info), (idx), ExpectedNameFrom(#method).c_str());                \
        return (env).Undefined();                                                                  \
    }

#define REQUIRE_ARG_ANY(env, info, idx, m1, m2)                                                    \
    if ((info).Length() <= (idx) || (!((info)[(idx)].m1() || (info)[(idx)].m2()))) {               \
        auto _expected = JoinExpected({#m1, #m2});                                                 \
        ThrowArgTypeError((env), (info), (idx), _expected.c_str());                                \
        return (env).Undefined();                                                                  \
    }

} // namespace node_snap7

#endif // SRC_NODE_SNAP7_HELPERS_H_
