/*
    Copyright (c) 2021 Alethea Katherine Flowers.
    Published under the standard MIT License.
    Full text available at: https://opensource.org/licenses/MIT
*/

#pragma once

/* Methods for obtaining information about the firmware build. */

#include <stdint.h>

struct GemBuildInfo {
    /*
        The release is the closest tag to the build's revision, aka - the most
        recent release when the build was created.

        Don't parse or compare to this string, the release year, month, and
        day are given separate properties if you want to programmatically
        check this release version.

        For example: 12.24.2020
    */
    const char* release;

    /* Year of the release, for example, 2020 */
    uint16_t release_year;
    /* Month of the release, Jan = 1 */
    uint8_t release_month;
    /* Day of the release. The 1st of the month = 1 */
    uint8_t release_day;

    /*
        The revision uniquely identifies the state of the working tree when
        the build was created.

        For example, if this is "12.24.2020" then the build was created from
        the tag 12.24.2020. However, if it's "12.24.2020-14-gb77c425-dirty"
        then the build was created from a commit that's 14 commits ahead
        of the 12.24.2020 tag and from a worktree that has uncommitted
        changes.
    */
    const char* revision;

    /*
        The date and time the build was created. For example:
        "20/01/2021 22:54 UTC"
    */
    const char* date;

    /*
        The name and version of the compiler used to create the build.
        For example: "arm-none-eabi-gcc 10.2.1 20201103 (release)"
    */
    const char* compiler;

    /*
        The name of the user and machine that created the build. For example:
        "stargirl@stargirls-mbp.lan".
    */
    const char* machine;
};

struct GemBuildInfo gem_build_info();
const char* gem_build_info_string();
