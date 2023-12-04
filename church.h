// https://en.wikipedia.org/wiki/Church_encoding

namespace church
{

// \f.\x.x
inline constexpr auto zero = []([[maybe_unused]] auto f) { return [](auto x) { return x; }; };

// \n.\f.\x.f (n f x)
inline constexpr auto succ = [](auto n) { return [=](auto f) { return [=](auto x) { return f(n(f)(x)); }; }; };

// \m.\n.\f.\x.m f (n f x)
inline constexpr auto plus = [](auto m) {
    return [=](auto n) { return [=](auto f) { return [=](auto x) { return m(f)(n(f)(x)); }; }; };
};

// \m.\n.\f.\x.m (n f) x
inline constexpr auto mult = [](auto m) {
    return [=](auto n) { return [=](auto f) { return [=](auto x) { return m(n(f))(x); }; }; };
};

// \m.\n.n m
inline constexpr auto exp = [](auto m) { return [=](auto n) { return n(m); }; };

// \n\f\x.n (\g.\h.h (g f)) (\u.x) (\u.u)
inline constexpr auto pred = [](auto n) {
    return [=](auto f) {
        return [=](auto x) {
            return n([=](auto g) { return [=](auto h) { return h(g(f)); }; })(
                [=]([[maybe_unused]] auto u) { return x; })([=](auto u) { return u; });
        };
    };
};

// \m.\n.n pred m
inline constexpr auto minus = [](auto m) { return [=](auto n) { return n(pred)(m); }; };

template<int I>
inline constexpr auto church = succ(church<I - 1>);

template<>
inline constexpr auto church<0> = zero;

inline constexpr int unchurch(auto n)
{
    return n([](int i) { return i + 1; })(0);
}

static_assert(unchurch(zero) == 0);
static_assert(unchurch(succ(zero)) == 1);
static_assert(unchurch(succ(succ(zero))) == 2);
static_assert(unchurch(church<5>) == 5);
static_assert(unchurch(plus(church<5>)(church<3>)) == 8);
static_assert(unchurch(mult(church<5>)(church<3>)) == 15);
static_assert(unchurch(exp(church<5>)(church<3>)) == 125);
static_assert(unchurch(pred(church<5>)) == 4);
static_assert(unchurch(minus(church<5>)(church<3>)) == 2);

} // namespace church
