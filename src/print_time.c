// vim:ts=4:sw=4:expandtab
#include <config.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include <yajl/yajl_gen.h>
#include <yajl/yajl_version.h>

#include "i3status.h"
#include "../gradient/gradient.h"

#define STRING_SIZE 500

static bool local_timezone_init = false;
static const char *local_timezone = NULL;
static const char *current_timezone = NULL;

void set_timezone(const char *tz) {
    if (!local_timezone_init) {
        /* First call, initialize. */
        local_timezone = getenv("TZ");
        local_timezone_init = true;
    }
    if (tz == NULL || tz[0] == '\0') {
        /* User wants localtime. */
        tz = local_timezone;
    }
    if (tz != current_timezone) {
        if (tz) {
            setenv("TZ", tz, 1);
        } else {
            unsetenv("TZ");
        }
        current_timezone = tz;
    }
    tzset();
}

void print_time(yajl_gen json_gen, char *buffer, const char *title, const char *format, const char *tz, const char *locale, const char *format_time, bool hide_if_equals_localtime, time_t t) {
    char *outwalk = buffer;
    struct tm local_tm, tm;

    if (title != NULL)
        INSTANCE(title);

    set_timezone(NULL);
    localtime_r(&t, &local_tm);

    set_timezone(tz);
    localtime_r(&t, &tm);

    // When hide_if_equals_localtime is true, compare local and target time to display only if different
    time_t local_t = mktime(&local_tm);
    double diff = difftime(local_t, t);
    if (hide_if_equals_localtime && diff == 0.0) {
        goto out;
    }

    if (locale != NULL) {
        setlocale(LC_ALL, locale);
    }

    char string_time[STRING_SIZE];

    if (format_time == NULL) {
        outwalk += strftime(buffer, 4096, format, &tm);
    } else {
        strftime(string_time, sizeof(string_time), format_time, &tm);
        placeholder_t placeholders[] = {
            {.name = "%time", .value = string_time}};

        const size_t num = sizeof(placeholders) / sizeof(placeholder_t);
        buffer = format_placeholders(format_time, &placeholders[0], num);
    }

    if (locale != NULL) {
        setlocale(LC_ALL, "");
    }

out:
    *outwalk = '\0';
    char colored_time[256] = "<span background='#";//123456
    char * c;
#define VOSHOD 6
#define ZAKAT 19
#define VOSHOD_COLOR YELLOW
#define POLDEN_COLOR WHITE
#define ZAKAT_COLOR RED
    if(tm.tm_hour < VOSHOD || tm.tm_hour > ZAKAT)
        c = hex(BLACK);
    else {
        double polovina = (ZAKAT+VOSHOD)/2;
        if(tm.tm_hour < polovina)
            c = hex(gradient(VOSHOD*60*60,VOSHOD_COLOR,polovina*60*60,POLDEN_COLOR,tm.tm_hour*60*60+tm.tm_min*60+tm.tm_sec));
        else
            c = hex(gradient(polovina*60*60,POLDEN_COLOR,ZAKAT*60*60,ZAKAT_COLOR,tm.tm_hour*60*60+tm.tm_min*60+tm.tm_sec));
    }
    strcat(colored_time,c);
    free(c);
    strcat(colored_time,"'>");
    strcat(colored_time,buffer);
    strcat(colored_time,"</span>");
    OUTPUT_FULL_TEXT(colored_time);
    if (format_time != NULL) {
        free(buffer);
    }
}
