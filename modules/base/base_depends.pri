if (!contains(IVW_MODULE_LIST, opengl)) {
    message("BaseModule: Resolving OpenGL module dependency.")
    IVW_MODULE_LIST += opengl
}
