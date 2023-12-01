// https://en.wikipedia.org/wiki/Church_encoding

namespace church
{

// \f.\x.x
constexpr auto zero = []([[maybe_unused]] auto f) { return [](auto x) { return x; }; };

// \n.\f.\x.f (n f x)
constexpr auto succ = [](auto n) { return [=](auto f) { return [=](auto x) { return f(n(f)(x)); }; }; };

// \m.\n.\f.\x.m f (n f x)
constexpr auto plus = [](auto m) {
    return [=](auto n) { return [=](auto f) { return [=](auto x) { return m(f)(n(f)(x)); }; }; };
};

// \m.\n.\f.\x.m (n f) x
constexpr auto mult = [](auto m) {
    return [=](auto n) { return [=](auto f) { return [=](auto x) { return m(n(f))(x); }; }; };
};

// \m.\n.n m
constexpr auto exp = [](auto m) { return [=](auto n) { return n(m); }; };

// \n\f\x.n (\g.\h.h (g f)) (\u.x) (\u.u)
constexpr auto pred = [](auto n) {
    return [=](auto f) {
        return [=](auto x) {
            return n([=](auto g) { return [=](auto h) { return h(g(f)); }; })(
                [=]([[maybe_unused]] auto u) { return x; })([=](auto u) { return u; });
        };
    };
};

// \m.\n.n pred m
constexpr auto minus = [](auto m) { return [=](auto n) { return n(pred)(m); }; };

template<int I>
struct church
{
    static constexpr auto value = succ(church<I - 1>::value);
};

template<>
struct church<0>
{
    static constexpr auto value = zero;
};

template<int I>
constexpr auto church_v = church<I>::value;

constexpr int unchurch(auto n)
{
    return n([](int i) { return i + 1; })(0);
}

static_assert(unchurch(zero) == 0);
static_assert(unchurch(succ(zero)) == 1);
static_assert(unchurch(succ(succ(zero))) == 2);
static_assert(unchurch(church_v<5>) == 5);
static_assert(unchurch(plus(church_v<5>)(church_v<3>)) == 8);
static_assert(unchurch(mult(church_v<5>)(church_v<3>)) == 15);
static_assert(unchurch(exp(church_v<5>)(church_v<3>)) == 125);
static_assert(unchurch(pred(church_v<5>)) == 4);
static_assert(unchurch(minus(church_v<5>)(church_v<3>)) == 2);

} // namespace church
