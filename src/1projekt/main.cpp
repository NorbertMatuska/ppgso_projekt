#include "window.h"

int main() {
    auto window = ParticleWindow{};

    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}
