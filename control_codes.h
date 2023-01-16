#pragma once

// control codes
enum CONTROL_CODE {
    END = 0,
    CONTINUE = 1,
    EXIST = 2,
    NOTEXIST = 3
} CONTROL_CODE_E;

// data tags
enum DATA {
    DATA = 0,
    CONTROL = 1
} DATA_TAG_E;

static const int control_codes_data[] = {
    (int)END,
    (int)CONTINUE,
    (int)EXIST,
    (int)NOTEXIST
};