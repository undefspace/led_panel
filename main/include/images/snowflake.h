#include <stdint.h>
#include <olive.h>

const uint32_t snowflake_map[] = {
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x29ffffff, 0xccffffff, 0x29ffffff, 0x0, 0x77fefefe, 0xccffffff, 0x27ffffff, 0x29ffffff, 0xccffffff, 0x23ffffff, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x16fefefe, 0xf5fefefe, 0xcafefefe, 0x0, 0x15fefefe, 0xf4fefefe, 0xb5fefefe, 0xbbffffff, 0xf5fefefe, 0x16fefefe, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x20ffffff, 0x40f3f3f3, 0xb3fafafa, 0xffffffff, 0x97fefefe, 0x66ffffff, 0xbbffffff, 0xffffffff, 0xffffffff, 0x8ffefefe, 0x0, 0x4ffffff, 0x0, 0x0,
    0x0, 0x9fefefe, 0xdafefefe, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x1ffefefe, 0x1ffffff, 0xaefefefe, 0x6ffffff, 0x0,
    0x0, 0xaffffff, 0x33ffffff, 0x33ffffff, 0xa8fefefe, 0xffffffff, 0x66ffffff, 0x33ffffff, 0x33ffffff, 0x64ffffff, 0xffffffff, 0x7fffffff, 0x6dffffff, 0xffffffff, 0x3fffffff, 0x0,
    0x0, 0x0, 0x0, 0x23ffffff, 0xf5fefefe, 0xb8fefefe, 0x0, 0x0, 0x0, 0x0, 0xbcfefefe, 0xf5fefefe, 0xedfefefe, 0xc0ffffff, 0x0, 0x0,
    0x98fdfdfd, 0xfbfefefe, 0xffffffff, 0xffffffff, 0xf7fefefe, 0x28ffffff, 0x0, 0x0, 0x0, 0x0, 0x29ffffff, 0xfefefefe, 0xffffffff, 0xe0ffffff, 0xccffffff, 0x81fdfdfd,
    0x54ffffff, 0x99fefefe, 0xc3fefefe, 0xffffffff, 0xf8fefefe, 0x27ffffff, 0x0, 0x0, 0x0, 0x0, 0x2affffff, 0xffffffff, 0xe0ffffff, 0xccffffff, 0xccffffff, 0x7bfefefe,
    0x0, 0x0, 0xd2fefefe, 0xe5fefefe, 0xf7fefefe, 0xadffffff, 0x0, 0x0, 0x0, 0x0, 0xc0ffffff, 0xf5fefefe, 0x18ffffff, 0x0, 0x0, 0x0,
    0x0, 0x3fffffff, 0xffffffff, 0x57fefefe, 0x8dfefefe, 0xffffffff, 0x7bfefefe, 0x66ffffff, 0x66ffffff, 0x8afefefe, 0xffffffff, 0xbefefefe, 0x66ffffff, 0x66ffffff, 0xaffffff, 0x0,
    0x0, 0x0, 0x87fdfdfd, 0x0, 0x32ffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xaefefefe, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xa2ffffff, 0xffffffff, 0xffffffff, 0x93fdfdfd, 0x33ffffff, 0x84fefefe, 0xffffffff, 0x9bfefefe, 0x33ffffff, 0x14ffffff, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x29ffffff, 0xffffffff, 0xa8fefefe, 0xc2ffffff, 0xebfefefe, 0x10ffffff, 0x1ffffff, 0xcefefefe, 0xf3fefefe, 0x14ffffff, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x29ffffff, 0xccffffff, 0x1bfefefe, 0x2dffffff, 0xccffffff, 0x67fefefe, 0x0, 0x2cffffff, 0xc0ffffff, 0xcffffff, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
};

const Olivec_Canvas snowflake = {
    .pixels = snowflake_map,
    .width = 16,
    .stride = 16,
    .height = 16,
};
