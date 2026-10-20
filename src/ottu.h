#ifndef __OTTU_H
#define __OTTU_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef OTTU_SERIAL_INTF_BAUDRATE
#define OTTU_SERIAL_INTF_BAUDRATE 115200
#endif

#ifndef OTTU_FRAMED_OUTPUT_ENABLED
#define OTTU_FRAMED_OUTPUT_ENABLED 1
#endif

#if (OTTU_FRAMED_OUTPUT_ENABLED == 1)

#define OTTU_STRINGIFY_IMPL(value) #value
#define OTTU_STRINGIFY(value) OTTU_STRINGIFY_IMPL(value)

#ifndef OTTU_TEST_FRAME_START
#ifndef OTTU_TEST_FRAME_TOKEN
#define OTTU_TEST_FRAME_START "=== ottu begin ==="
#else
#define OTTU_TEST_FRAME_START \
    "=== ottu begin " OTTU_STRINGIFY(OTTU_TEST_FRAME_TOKEN) " ==="
#endif
#endif

#ifndef OTTU_TEST_FRAME_END
#ifndef OTTU_TEST_FRAME_TOKEN
#define OTTU_TEST_FRAME_END "=== ottu end ==="
#else
#define OTTU_TEST_FRAME_END \
    "=== ottu end " OTTU_STRINGIFY(OTTU_TEST_FRAME_TOKEN) " ==="
#endif
#endif

#else
#ifndef OTTU_TEST_FRAME_START
#define OTTU_TEST_FRAME_START ""
#endif
#ifndef OTTU_TEST_FRAME_END
#define OTTU_TEST_FRAME_END ""
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* __OTTU_H */
