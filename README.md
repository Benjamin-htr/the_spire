# The spire

This project was an ISEN study project c 2022 inspired by the game 'Slay the Spire'. You can read the instructions of our teacher [here](AP3_C_Projet_2021-2022.pdf) (in French).
The instructions were to create a turn-based card game with C language and there were no guidance on the aesthetic. Thus, we spiced it up and decided to make real graphics with animations and sound effects using the raylib library (and not only terminal ui as expected).

## Targeted Platforms

At the original time of development, the project was targeted to be built on desktop platforms (Windows). But I recently decided to port it to the web platform using Emscripten to make it more accessible. You can test the web version [here](https://the-spire.benjamin-hautier.com).

## Building for Web Platform

To build the project for the web platform using Emscripten, ensure you have Emscripten installed and activated in your environment.
You can then run the following command:

```bash
make PLATFORM=PLATFORM_WEB
```

This will compile the project and generate the necessary HTML, JavaScript, and WebAssembly files in the build directory.

## Custom Shell and JavaScript Integration

You can customize the web shell by modifying the HTML file located in `src/web_shell/shell.html`.

Additionally, you can include custom JavaScript files to be executed before and after the main module code by placing them in the `src/web_shell/` directory and updating the `BUILD_WEB_PRE_JS_FILES` and `BUILD_WEB_POST_JS_FILES` variables in the Makefile.
