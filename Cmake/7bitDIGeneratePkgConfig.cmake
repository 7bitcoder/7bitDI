configure_file("cmake/7bitDI.pc.in" "7bitDI.pc" @ONLY)

install(FILES "${PROJECT_BINARY_DIR}/7bitDI.pc" DESTINATION "lib/pkgconfig")