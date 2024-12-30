To compile a program with this library you have to link against libbacktrace (it is already installed by default on linux systems)

example

```
g++ main.cpp -L /path/to/Stacktracer -lStackTracer -lbacktrace

```

