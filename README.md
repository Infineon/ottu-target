# ottu-target

`ottu-target` is a C library target for host-based testing with
[ottu](https://github.com/Infineon/ottu).

It currently provides a [Unity](https://github.com/ThrowTheSwitch/Unity)-based
test fixture through `ottunity`. The
fixture simplifies test creation with support macros, so tests do not need to
define groups or test runners explicitly.

## Build

Clone the repository with its Unity dependency:

```sh
git clone --recurse-submodules https://github.com/Infineon/ottu-target.git
cd ottu-target
cmake -S . -B build
cmake --build build
```

Examples are built by default. Disable them with:

```sh
cmake -S . -B build -DOTTU_BUILD_EXAMPLES=OFF
```

## Example

Include `ottunity.h`, define tests with `OTTUNITY_TEST`, and add
`OTTUNITY_RUN()` once:

```c
#include "ottunity.h"

OTTUNITY_TEST(is_true)
{
    TEST_ASSERT_TRUE(1);
}

OTTUNITY_RUN();
```

Link the test executable against the `ottunity` CMake target.

## Toolchain requirements

OTTUNITY automatically registers groups, tests, setup functions, and teardown
functions through compiler constructor attributes. Use a GCC- or
Clang-compatible toolchain that supports `__attribute__((constructor))`.

## OTTUNITY macro responsibilities

OTTUNITY provides defaults for the parts of a test that do not need custom
behavior. Compared with using the Unity fixture directly, this removes the
boilerplate required to create groups, register tests, define test runners,
and provide empty setup or teardown functions. Tests can focus on their test
body while OTTUNITY handles registration and execution through
`OTTUNITY_RUN()`. Unity assertions and the underlying Unity test fixture
remain available.

| Macro or definition | Required? | Responsibility |
| --- | --- | --- |
| `OTTUNITY_TEST(name)` | Yes, for each test | Declares a test and registers it with the automatic group. |
| `OTTUNITY_RUN()` | Yes, once per test executable | Starts the test suite. |
| `OTTUNITY_GROUP(group)` | Optional | Declares an explicit group. If it is omitted, `OTTUNITY_TEST()` uses the automatic group. |
| `OTTUNITY_TEST_SETUP()` / `OTTUNITY_GROUP_TEST_SETUP(group)` | Optional | Defines setup code. If omitted, OTTUNITY uses an empty setup function. |
| `OTTUNITY_TEST_TEAR_DOWN()` / `OTTUNITY_GROUP_TEST_TEAR_DOWN(group)` | Optional | Defines teardown code. If omitted, OTTUNITY uses an empty teardown function. |
| Test runner definition | Not required | OTTUNITY creates and registers the runner for each test declared with `OTTUNITY_TEST()` or `OTTUNITY_GROUP_TEST()`. |

For a basic test file, only the test declarations and one `OTTUNITY_RUN()`
call are needed. Explicit groups, setup, and teardown functions are only
needed when the test suite requires them.

## Configuration

Create a project-specific `ottunity_config.h` and add its directory to the
include path. The file is included automatically when it is available and can
override defaults such as `OTTUNITY_SUITE_GROUP_NUM_MAX`,
`OTTUNITY_GROUP_TEST_NUM_MAX`, `OTTUNITY_ENV_SETUP()`, and `OTTUNITY_RUN()`.

For example:

```c
#define OTTUNITY_SUITE_GROUP_NUM_MAX 32
#define OTTUNITY_GROUP_TEST_NUM_MAX 64
```

These limits control the maximum number of groups in a suite and tests in
each group. Increase them in the project configuration when a test suite
needs more capacity. Groups or tests declared beyond the configured limits
are currently ignored without generating a runtime error.

The repository does not require a default configuration header. Applications
can provide their own configuration header when they need to override these
defaults. On compilers without `__has_include`, specify the configuration
header explicitly with `OTTUNITY_CONFIG_FILE`, for example:

```sh
-DOTTUNITY_CONFIG_FILE=\"my_ottunity_config.h\"
```

## Unity output on an embedded target

Unity uses `putchar()` by default. For an embedded target, provide a Unity
configuration header and define `UNITY_OUTPUT_CHAR` to send one character to
the target UART:

```c
/* unity_config.h */
#define UNITY_OUTPUT_CHAR_HEADER_DECLARATION uart_write_char(int)
#define UNITY_OUTPUT_CHAR(a) uart_write_char(a)

#define UNITY_OUTPUT_FLUSH_HEADER_DECLARATION uart_flush(void)
#define UNITY_OUTPUT_FLUSH() uart_flush()
```

Declare or implement the UART functions in the target platform code:

```c
void uart_write_char(int character)
{
    uart_put_byte((unsigned char)character);
}

void uart_flush(void)
{
    uart_wait_until_empty();
}
```

Make Unity include this configuration when compiling both the Unity library
and the test application:

```cmake
target_compile_definitions(unity PUBLIC UNITY_INCLUDE_CONFIG_H)
target_include_directories(unity PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/config)
```

For non-CMake builds, add `-DUNITY_INCLUDE_CONFIG_H` and the directory
containing `unity_config.h` to the compiler include paths. The exact UART
initialization and transmit functions depend on the embedded platform.

### Framed test output

The `ottu.h` header provides `OTTU_TEST_FRAME_START` and
`OTTU_TEST_FRAME_END` string macros for marking the boundaries of a test run.
Framing is enabled by default, and the default marker lines are:

```text
=== ottu begin ===
...
=== ottu end ===
```

The test application prints these markers around the Unity output. A host can
ignore serial data until it reads the begin marker, then collect output through
the matching end marker.

To add a build-time token to both markers, pass it to the compiler as a
preprocessor definition. For example, in a CMake-based toolchain:

```cmake
target_compile_definitions(my_test_target PRIVATE OTTU_TEST_FRAME_TOKEN=42)
```

This produces `=== ottu begin 42 ===` and `=== ottu end 42 ===`. The token is
compiled into the firmware and is not generated separately for each run. If
the token is not defined, the markers omit it.

Framing is enabled by default. To disable it for a target, add:

```cmake
target_compile_definitions(my_test_target PRIVATE OTTU_FRAMED_OUTPUT_ENABLED=0)
```
