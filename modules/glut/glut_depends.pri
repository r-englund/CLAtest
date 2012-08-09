if (!contains(IVW_MODULE_LIST, opengl)) {
    message("GLUTModule: Resolving OpenGL module dependency.")
    IVW_MODULE_LIST += opengl
}
