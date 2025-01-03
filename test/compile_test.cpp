#define DOCTEST_CONFIG_IMPLEMENT

#include <doctest.h>

int compile_test_function() {
    return 1 + 3;
}

TEST_CASE("Compile test") {
    compile_test_function();
}

int main() {
    doctest::Context context;

    context.setOption("success", true);

    return context.run();
}