# ottu-target

`ottu-target` is a C library target for host-based testing with
[ottu](https://github.com/Infineon/ottu).

It currently provides a [Unity](https://github.com/ThrowTheSwitch/Unity)-based
test fixture through `ottunity`. The
fixture simplifies test creation with support macros, so tests do not need to
define groups or test runners explicitly (see
[ottunity versus Unity fixture](#ottunity-versus-unity-fixture)).

## ottunity API reference

The public macros are defined in [`ottunity.h`](src/ottunity/ottunity.h).
Use the basic API when a test file needs only the default group, or the group
API when tests need explicit grouping or group-specific setup and teardown.

| Macro | Responsibility |
| --- | --- |
| `OTTUNITY_TEST(name)` | Declares a test in the automatic group. |
| `OTTUNITY_TEST_SETUP()` / `OTTUNITY_TEST_TEAR_DOWN()` | Define setup or teardown for the automatic group. Both are optional; an empty function is used when omitted. |
| `OTTUNITY_RUN()` | Defines the test executable's `main()` and runs the suite. Use once per test executable. The default implementation calls `OTTUNITY_ENV_SETUP()` before the suite runner. |
| `OTTUNITY_GROUP(group)` | Declares and registers an explicit test group. |
| `OTTUNITY_GROUP_TEST(group, name)` | Declares a test in an explicit group. |
| `OTTUNITY_GROUP_TEST_SETUP(group)` / `OTTUNITY_GROUP_TEST_TEAR_DOWN(group)` | Define optional setup or teardown for an explicit group. |

The automatic group is named from the source file containing
`OTTUNITY_RUN()`. For a basic test file, declare tests and include one
`OTTUNITY_RUN()` call; add groups or setup and teardown functions only when
needed.

Assertions such as `TEST_ASSERT_TRUE` are provided by Unity, not ottunity.
See the [Unity Assertions Reference](https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsReference.md)
for the available assertion macros and their behavior.

## Example

Include `ottunity.h`, define tests with `OTTUNITY_TEST`, and add
`OTTUNITY_RUN()` once:

```c
#include "ottunity.h"

OTTUNITY_TEST(is_true) {
    TEST_ASSERT_TRUE(1);
}

OTTUNITY_RUN();
```

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

## Toolchain requirements

ottunity automatically registers groups, tests, setup functions, and teardown
functions through compiler constructor attributes. Use a GCC- or
Clang-compatible toolchain that supports `__attribute__((constructor))`.

## ottunity versus Unity fixture

ottunity builds on the Unity fixture, so the fixture's test concepts still
apply. These ottunity macros provide the corresponding group, test, and
setup/teardown declarations. See the [Unity fixture documentation](libs/unity/extras/fixture/readme.md)
for its usage model:

| Unity fixture macro | ottunity macro | Relationship |
| --- | --- | --- |
| `TEST_GROUP(group)` | `OTTUNITY_GROUP(group)` | Declares a group. ottunity also registers it with the suite. |
| `TEST_SETUP(group)` / `TEST_TEAR_DOWN(group)` | `OTTUNITY_GROUP_TEST_SETUP(group)` / `OTTUNITY_GROUP_TEST_TEAR_DOWN(group)` | Define group setup or teardown. ottunity supplies no-op defaults when these are omitted. |
| `TEST(group, name)` | `OTTUNITY_GROUP_TEST(group, name)` | Declares the test body. ottunity also registers the test and creates its runner. |

For tests in the automatic group, `OTTUNITY_TEST(name)` and its setup/teardown
macros are the shorthand forms; no explicit group declaration is needed.

When using ottunity, you do not need to write the Unity Fixture's
`TEST_GROUP_RUNNER` and `RUN_TEST_CASE` calls to enumerate tests, or call
`RUN_TEST_GROUP` from a manually defined `main()`. ottunity registers tests
automatically and `OTTUNITY_RUN()` supplies the default entry point and suite
execution. Unity assertions remain available unchanged.
