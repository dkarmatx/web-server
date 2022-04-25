mkdir -p subprojects
meson wrap install gtest

meson setup builddir && ninja -C builddir