#include "BlinkerApi.h"


static blinker_widgets_str_t   _Widgets_str[BLINKER_MAX_WIDGET_SIZE];
static blinker_widgets_rgb_t   _Widgets_rgb[BLINKER_MAX_WIDGET_SIZE/2];
static blinker_widgets_int_t   _Widgets_int[BLINKER_MAX_WIDGET_SIZE];
static blinker_widgets_tab_t   _Widgets_tab[BLINKER_MAX_WIDGET_SIZE];
static uint8_t _wCount_str = 0;
static uint8_t _wCount_rgb = 0;
static uint8_t _wCount_int = 0;
static uint8_t _wCount_tab = 0;

static blinker_callback_with_json_arg_t blinker_data_func = NULL;

static bool blinker_parsed = false;
static bool blinker_auto_format = false;
static uint32_t blinker_auto_format_time = 0;
static char blinker_send_buf[BLINKER_MAX_SEND_SIZE] = { 0 };
static int  raw_data[4] = {0,0,0,0};
static uint8_t raw_len = 0;

int8_t check_string_num(const char *name, const blinker_widgets_str_t *c, uint8_t count)
{
    for (uint8_t cNum = 0; cNum < count; cNum++)
    {
        if (strcmp(c[cNum].name, name) == 0)
        {
            // // BLINKER_LOG_ALL("check_string_num, name: %s, num: %d", name, cNum);
            return cNum;
        }
    }

    return BLINKER_OBJECT_NOT_AVAIL;
}

int8_t check_rgb_num(const char *name, const blinker_widgets_rgb_t *c, uint8_t count)
{
    for (uint8_t cNum = 0; cNum < count; cNum++)
    {
        if (strcmp(c[cNum].name, name) == 0)
        {
            // // BLINKER_LOG_ALL("check_rgb_num, name: %s, num: %d", name, cNum);
            return cNum;
        }
    }

    return BLINKER_OBJECT_NOT_AVAIL;
}

int8_t check_int_num(const char *name, const blinker_widgets_int_t *c, uint8_t count)
{
    for (uint8_t cNum = 0; cNum < count; cNum++)
    {
        if (strcmp(c[cNum].name, name) == 0)
        {
            // // BLINKER_LOG_ALL("check_rgb_num, name: %s, num: %d", name, cNum);
            return cNum;
        }
    }

    return BLINKER_OBJECT_NOT_AVAIL;
}

int8_t check_tab_num(const char *name, const blinker_widgets_tab_t *c, uint8_t count)
{
    for (uint8_t cNum = 0; cNum < count; cNum++)
    {
        if (strcmp(c[cNum].name, name) == 0)
        {
            // // BLINKER_LOG_ALL("check_rgb_num, name: %s, num: %d", name, cNum);
            return cNum;
        }
    }

    return BLINKER_OBJECT_NOT_AVAIL;
}

uint8_t attach_widget_string(const char *_name, blinker_callback_with_string_arg_t _func)
{
    int8_t num = check_string_num(_name, _Widgets_str, _wCount_str);

    if (num == BLINKER_OBJECT_NOT_AVAIL)
    {
        if (_wCount_str < BLINKER_MAX_WIDGET_SIZE*2)
        {
            _Widgets_str[_wCount_str].name = _name;
            _Widgets_str[_wCount_str].wfunc = _func;
            _wCount_str++;

            // BLINKER_LOG_ALL("new widgets: %s, _wCount_str: %d", _name, _wCount_str);
            return _wCount_str;
        }
        else
        {
            return 0;
        }
    }
    else if(num >= 0 )
    {
        // BLINKER_ERR_LOG("widgets name > %s < has been registered, please register another name!", _name);
        return 0;
    }
    else
    {
        return 0;
    }
}

uint8_t attach_widget_rgb(const char *_name, blinker_callback_with_rgb_arg_t _func)
{
    int8_t num = check_rgb_num(_name, _Widgets_rgb, _wCount_rgb);

    if (num == BLINKER_OBJECT_NOT_AVAIL)
    {
        if (_wCount_rgb < BLINKER_MAX_WIDGET_SIZE*2)
        {
            _Widgets_rgb[_wCount_rgb].name = _name;
            _Widgets_rgb[_wCount_rgb].wfunc = _func;
            _wCount_rgb++;

            // BLINKER_LOG_ALL("new widgets: %s, _wCount_rgb: %d", _name, _wCount_rgb);
            return _wCount_rgb;
        }
        else
        {
            return 0;
        }
    }
    else if(num >= 0 )
    {
        // BLINKER_ERR_LOG("widgets name > %s < has been registered, please register another name!", _name);
        return 0;
    }
    else
    {
        return 0;
    }
}

uint8_t attach_widget_int(const char *_name, blinker_callback_with_int32_arg_t _func)
{
    int8_t num = check_int_num(_name, _Widgets_int, _wCount_int);

    if (num == BLINKER_OBJECT_NOT_AVAIL)
    {
        if (_wCount_int < BLINKER_MAX_WIDGET_SIZE*2)
        {
            _Widgets_int[_wCount_int].name = _name;
            _Widgets_int[_wCount_int].wfunc = _func;
            _wCount_int++;

            // BLINKER_LOG_ALL("new widgets: %s, _wCount_int: %d", _name, _wCount_int);
            return _wCount_int;
        }
        else
        {
            return 0;
        }
    }
    else if(num >= 0 )
    {
        // BLINKER_ERR_LOG("widgets name > %s < has been registered, please register another name!", _name);
        return 0;
    }
    else
    {
        return 0;
    }
}

uint8_t attach_widget_tab(const char *_name, blinker_callback_with_tab_arg_t _func, blinker_callback_t _func1)
{
    int8_t num = check_tab_num(_name, _Widgets_tab, _wCount_tab);

    if (num == BLINKER_OBJECT_NOT_AVAIL)
    {
        if (_wCount_tab < BLINKER_MAX_WIDGET_SIZE*2)
        {
            _Widgets_tab[_wCount_tab].name = _name;
            _Widgets_tab[_wCount_tab].wfunc = _func;
            _Widgets_tab[_wCount_tab].wfunc1 = _func1;
            _wCount_tab++;

            // BLINKER_LOG_ALL("new widgets: %s, _wCount_tab: %d", _name, _wCount_tab);
            return _wCount_tab;
        }
        else
        {
            return 0;
        }
    }
    else if(num >= 0 )
    {
        // BLINKER_ERR_LOG("widgets name > %s < has been registered, please register another name!", _name);
        return 0;
    }
    else
    {
        return 0;
    }
}

void blinker_button_print(const BlinkerButton *button, const blinker_button_config_t * config)
{
    cJSON *pValue = cJSON_CreateObject();
    // cJSON_AddStringToObject(pValue,"mac","xuhongv");

    if (config->state) cJSON_AddStringToObject(pValue, BLINKER_CMD_STATE, config->state);
    if (config->icon) cJSON_AddStringToObject(pValue, BLINKER_CMD_ICON, config->icon);
    if (config->color) cJSON_AddStringToObject(pValue, BLINKER_CMD_COLOR, config->color);
    if (config->color) cJSON_AddStringToObject(pValue, BLINKER_CMD_COLOR_, config->color);
    if (config->content) cJSON_AddStringToObject(pValue, BLINKER_CMD_CONTENT, config->content);
    if (config->text1) cJSON_AddStringToObject(pValue, BLINKER_CMD_TEXT, config->text1);
    if (config->text2) cJSON_AddStringToObject(pValue, BLINKER_CMD_TEXT1, config->text2);
    if (config->textColor) cJSON_AddStringToObject(pValue, BLINKER_CMD_TEXTCOLOR, config->textColor);

    char *_data;
    _data = cJSON_PrintUnformatted(pValue);
    cJSON_Delete(pValue);

    blinker_print(button->name, _data, 0);

    free(_data);
    // BLINKER_LOG_ALL("blinker_button_print");
}

void blinker_number_print(const BlinkerNumber *number, const blinker_number_config_t * config)
{
    cJSON *pValue = cJSON_CreateObject();

    if (config->icon) cJSON_AddStringToObject(pValue, BLINKER_CMD_ICON, config->icon);
    if (config->color) cJSON_AddStringToObject(pValue, BLINKER_CMD_COLOR, config->color);
    if (config->color) cJSON_AddStringToObject(pValue, BLINKER_CMD_COLOR_, config->color);
    if (config->unit) cJSON_AddStringToObject(pValue, BLINKER_CMD_UNIT, config->unit);
    if (config->text) cJSON_AddStringToObject(pValue, BLINKER_CMD_TEXT, config->text);
    // cJSON *value = cJSON_CreateNumber(config->value);
    // cJSON_AddItemToObject(pValue, BLINKER_CMD_VALUE, value);
    if (config->value) cJSON_AddStringToObject(pValue, BLINKER_CMD_VALUE, config->value);

    char *_data;
    _data = cJSON_PrintUnformatted(pValue);
    cJSON_Delete(pValue);

    blinker_print(number->name, _data, 0);
    
    free(_data);
}

void blinker_rgb_print(const BlinkerRGB *rgb, const blinker_rgb_config_t * config)
{
    // cJSON *pValue = cJSON_CreateObject();

    // cJSON_AddItemToObject(pValue, BLINKER_CMD_RGB, cJSON_CreateIntArray(config->rgbw, 4));
    char data[24] = {0};

    sprintf(data, "[%d,%d,%d,%d]", config->rgbw[0], config->rgbw[1], config->rgbw[2], config->rgbw[3]);

    // cJSON *pValue = cJSON_CreateObject();

    // char _data[22] = {0};

    // // cJSON_AddItemToObject(pValue, BLINKER_CMD_RGB, cJSON_CreateIntArray(rrgg, 4));

    // sprintf(_data, "[%d, %d, %d, %d]", config->rgbw[0], config->rgbw[1], config->rgbw[2], config->rgbw[3]);

    // cJSON_AddRawToObject(pValue, BLINKER_CMD_RGB, data);

    // // BLINKER_LOG_ALL("%s", cJSON_PrintUnformatted(pValue));

    // cJSON_Delete(pValue);

    raw_len = 4;
    raw_data[0] = config->rgbw[0];
    raw_data[1] = config->rgbw[1];
    raw_data[2] = config->rgbw[2];
    raw_data[3] = config->rgbw[3];

    // BLINKER_LOG_ALL("rgb: %d, %d, %d, %d", raw_data[0], raw_data[1], raw_data[2], raw_data[3]);

    blinker_print(rgb->name, data, 1);

    // cJSON_Delete(pValue);
}

void blinker_slider_print(const BlinkerSlider *slider, const blinker_slider_config_t * config)
{
    cJSON *pValue = cJSON_CreateObject();

    if (config->color) cJSON_AddStringToObject(pValue, BLINKER_CMD_COLOR, config->color);
    if (config->color) cJSON_AddStringToObject(pValue, BLINKER_CMD_COLOR_, config->color);
    if (config->value) cJSON_AddStringToObject(pValue, BLINKER_CMD_VALUE, config->value);

    char *_data;
    _data = cJSON_PrintUnformatted(pValue);
    cJSON_Delete(pValue);

    blinker_print(slider->name, _data, 0);
    
    free(_data);
}

void blinker_switch_print(const blinker_switch_config_t * config)
{
    // cJSON *pValue = cJSON_CreateObject();

    // if (config->state) cJSON_AddStringToObject(pValue, BLINKER_CMD_BUILTIN_SWITCH, config->state);

    // char _data[128] = {0};
    // cJSON_PrintPreallocated(pValue, _data, 128, 0);
    blinker_print(BLINKER_CMD_BUILTIN_SWITCH, config->state, 0);

    // cJSON_Delete(pValue);
}

void blinker_tab_print(const BlinkerTab *tab, const blinker_tab_config_t * config)
{
    cJSON *pValue = cJSON_CreateObject();

    // cJSON_AddItemToObject(pValue, BLINKER_CMD_RGB, cJSON_CreateIntArray(config->rgbw, 4));
    char data[24] = {0};

    sprintf(data, "%d%d%d%d%d", config->tab[0], config->tab[1], config->tab[2], config->tab[3], config->tab[4]);

    cJSON_AddStringToObject(pValue, BLINKER_CMD_VALUE, data);

    // cJSON *pValue = cJSON_CreateObject();

    // char _data[22] = {0};

    // // cJSON_AddItemToObject(pValue, BLINKER_CMD_RGB, cJSON_CreateIntArray(rrgg, 4));

    // sprintf(_data, "[%d, %d, %d, %d]", config->rgbw[0], config->rgbw[1], config->rgbw[2], config->rgbw[3]);

    // cJSON_AddRawToObject(pValue, BLINKER_CMD_RGB, data);

    // // BLINKER_LOG_ALL("%s", cJSON_PrintUnformatted(pValue));
    char *_data;
    _data = cJSON_PrintUnformatted(pValue);
    cJSON_Delete(pValue);

    blinker_print(tab->name, _data, 0);
    
    free(_data);

    // cJSON_Delete(pValue);
}

void blinker_text_print(const BlinkerText *text, const blinker_text_config_t * config)
{
    cJSON *pValue = cJSON_CreateObject();

    if (config->icon) cJSON_AddStringToObject(pValue, BLINKER_CMD_ICON, config->icon);
    if (config->color) cJSON_AddStringToObject(pValue, BLINKER_CMD_COLOR, config->color);
    if (config->color) cJSON_AddStringToObject(pValue, BLINKER_CMD_COLOR_, config->color);
    if (config->text) cJSON_AddStringToObject(pValue, BLINKER_CMD_TEXT, config->text);
    if (config->text1) cJSON_AddStringToObject(pValue, BLINKER_CMD_VALUE, config->text1);

    char *_data;
    _data = cJSON_PrintUnformatted(pValue);
    cJSON_Delete(pValue);

    blinker_print(text->name, _data, 0);
    
    free(_data);
}

void blinker_button_init(BlinkerButton *button, blinker_callback_with_string_arg_t _func)
{
    button->wNum = attach_widget_string(button->name, _func);
}

void blinker_rgb_init(BlinkerRGB *rgb, blinker_callback_with_rgb_arg_t _func)
{
    rgb->wNum = attach_widget_rgb(rgb->name, _func);
}

void blinker_slider_init(BlinkerSlider *slider, blinker_callback_with_int32_arg_t _func)
{
    slider->wNum = attach_widget_int(slider->name, _func);
}

void blinker_switch_init(blinker_callback_with_string_arg_t _func)
{
    attach_widget_string(BLINKER_CMD_BUILTIN_SWITCH, _func);
}

void blinker_tab_init(BlinkerTab *tab, blinker_callback_with_tab_arg_t _func, blinker_callback_t _func1)
{
    tab->wNum = attach_widget_tab(tab->name, _func, _func1);
}

void widget_string_parse(const char *_wName, cJSON *data)
{
    // BLINKER_LOG_ALL("_Widgets_str _wName: %s", _wName);

    int8_t num = check_string_num(_wName, _Widgets_str, _wCount_str);

    if (num == BLINKER_OBJECT_NOT_AVAIL) return;

    cJSON *state = cJSON_GetObjectItem(data, _wName);

    // // BLINKER_LOG_ALL("_Widgets_str num: %d", num);

    // BLINKER_LOG_ALL("_Widgets_str null: %d", state->string == NULL);

    if (state->type == cJSON_String)
    {
        // BLINKER_LOG_ALL("widget_string_parse isParsed");

        blinker_parsed = true;

        // BLINKER_LOG_ALL("widget_string_parse: %s", _wName);

        blinker_callback_with_string_arg_t nbFunc = _Widgets_str[num].wfunc;

        if (nbFunc) nbFunc(state->valuestring);
    }

    // cJSON_Delete(state);
}

void widget_rgb_parse(const char *_wName, cJSON *data)
{
    // BLINKER_LOG_ALL("_Widgets_rgb _wName: %s", _wName);

    int8_t num = check_rgb_num(_wName, _Widgets_rgb, _wCount_rgb);

    if (num == BLINKER_OBJECT_NOT_AVAIL) return;

    cJSON *state = cJSON_GetObjectItem(data, _wName);

    // BLINKER_LOG_ALL("_Widgets_rgb num: %d", num);

    if (state->type == cJSON_Array)
    {
        // BLINKER_LOG_ALL("widget_rgb_parse isParsed");

        blinker_parsed = true;

        // BLINKER_LOG_ALL("widget_rgb_parse: %s", _wName);

        blinker_callback_with_rgb_arg_t nbFunc = _Widgets_rgb[num].wfunc;

        if (nbFunc) nbFunc(cJSON_GetArrayItem(state, 0)->valueint, cJSON_GetArrayItem(state, 1)->valueint, cJSON_GetArrayItem(state, 2)->valueint, cJSON_GetArrayItem(state, 3)->valueint);
    }

    // cJSON_Delete(state);
}

void widget_int_parse(const char *_wName, cJSON *data)
{
    // BLINKER_LOG_ALL("_Widgets_int _wName: %s", _wName);

    int8_t num = check_int_num(_wName, _Widgets_int, _wCount_int);

    if (num == BLINKER_OBJECT_NOT_AVAIL) return;

    cJSON *state = cJSON_GetObjectItem(data, _wName);

    // BLINKER_LOG_ALL("_Widgets_int num: %d", num);

    if (state->type == cJSON_Number)
    {
        // BLINKER_LOG_ALL("widget_int_parse isParsed");

        blinker_parsed = true;

        // BLINKER_LOG_ALL("widget_int_parse: %s", _wName);

        blinker_callback_with_int32_arg_t nbFunc = _Widgets_int[num].wfunc;

        if (nbFunc) nbFunc(state->valueint);
    }

    // cJSON_Delete(state);
}

void widget_tab_parse(const char *_wName, cJSON *data)
{
    // BLINKER_LOG_ALL("_Widgets_tab _wName: %s", _wName);

    int8_t num = check_tab_num(_wName, _Widgets_tab, _wCount_tab);

    if (num == BLINKER_OBJECT_NOT_AVAIL) return;

    cJSON *state = cJSON_GetObjectItem(data, _wName);

    // BLINKER_LOG_ALL("_Widgets_tab num: %d", num);

    if (state->type == cJSON_String)
    {
        // BLINKER_LOG_ALL("widget_tab_parse isParsed");

        blinker_parsed = true;

        // BLINKER_LOG_ALL("widget_tab_parse: %s", _wName);

        blinker_callback_with_tab_arg_t nbFunc = _Widgets_tab[num].wfunc;

        // if (nbFunc) nbFunc(atoi(state->valuestring));

        // char tab_data[10] = {0};

        // cJSON_PrintPreallocated(state, tab_data, 10, 0);
        
        char *tab_data;
        tab_data = cJSON_PrintUnformatted(state);
        // cJSON_Delete(pValue);

        // BLINKER_LOG_ALL("tab_data: %c", tab_data[1]);

        if (nbFunc)
        {
            for (uint8_t num = 0; num < 5; num++)
            {
                // BLINKER_LOG_ALL("num: %c", tab_data[num + 1]);

                if (tab_data[num + 1] == '1')
                {
                    if (nbFunc) {
                        switch (num)
                        {
                            case 0:
                                nbFunc(BLINKER_CMD_TAB_0);
                                break;
                            case 1:
                                nbFunc(BLINKER_CMD_TAB_1);
                                break;
                            case 2:
                                nbFunc(BLINKER_CMD_TAB_2);
                                break;
                            case 3:
                                nbFunc(BLINKER_CMD_TAB_3);
                                break;
                            case 4:
                                nbFunc(BLINKER_CMD_TAB_4);
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
        }

        free(tab_data);
    }

    blinker_callback_t wFunc1 = _Widgets_tab[num].wfunc1;
    if (wFunc1) {
        wFunc1();
    }

    // cJSON_Delete(state);
}

void widget_parse(cJSON *data)
{
    // BLINKER_LOG_ALL("widget_parse");

    for (uint8_t wNum = 0; wNum < _wCount_str; wNum++) {
        if (blinker_parsed) return;
        widget_string_parse(_Widgets_str[wNum].name, data);
    }
    for (uint8_t wNum = 0; wNum < _wCount_rgb; wNum++) {
        if (blinker_parsed) return;
        widget_rgb_parse(_Widgets_rgb[wNum].name, data);
    }
    for (uint8_t wNum = 0; wNum < _wCount_int; wNum++) {
        if (blinker_parsed) return;
        widget_int_parse(_Widgets_int[wNum].name, data);
    }
    for (uint8_t wNum = 0; wNum < _wCount_tab; wNum++) {
        if (blinker_parsed) return;
        widget_tab_parse(_Widgets_tab[wNum].name, data);
    }
}

void blinker_attach_data(blinker_callback_with_json_arg_t func)
{
    blinker_data_func = func;
}

// void blinker_check_auto_format(void)
// {
//     if (blinker_auto_format)
//     {
//         if ((blinker_millis() - blinker_auto_format_time) >= BLINKER_MSG_AUTOFORMAT_TIMEOUT)
//         {
//             // // BLINKER_LOG_ALL("blinker_check_auto_format");
//             if (strlen(blinker_send_buf))
//             {
//                 // // BLINKER_LOG_ALL("print: %s", blinker_send_buf);
//                 uint8_t need_check = true;
//                 if (strstr(blinker_send_buf, BLINKER_CMD_CONNECTED)) need_check = false;
//                 blinker_ble_print(blinker_send_buf, need_check);
//             }
//             free(blinker_send_buf);
//             blinker_auto_format = false;
//         }
//     }
// }

void blinker_check_format(void)
{
    if (!blinker_auto_format)
    {
        blinker_auto_format = true;

        // blinker_send_buf = (char*)malloc(BLINKER_MAX_SEND_SIZE*sizeof(char));
        memset(blinker_send_buf, '\0', BLINKER_MAX_SEND_SIZE);
    }
}

void blinker_auto_format_data(const char * key, const char * value, int8_t isRaw)
{
    // BLINKER_LOG_ALL("auto_format_data key: %s, value: %s", key, value);

    if (strlen(blinker_send_buf))
    {
        cJSON *root = cJSON_Parse(blinker_send_buf);

        if (root != NULL)
        {
            cJSON *check_key = cJSON_GetObjectItem(root, key);
            if (check_key != NULL)
            {
                cJSON_DeleteItemFromObject(root, key);
            }
        }

        if (isJson(value) && !isRaw)
        {
            cJSON *new_item = cJSON_Parse(value);
            cJSON_AddItemToObject(root, key, new_item);
        }
        else
        {
            if (isRaw) 
            {
                // cJSON_AddRawToObject(root, key, value);
                
                cJSON_AddItemToObject(root, key, cJSON_CreateIntArray(raw_data, raw_len));
                // BLINKER_LOG_ALL("cJSON_AddRawToObject");
            }
            else 
            {
                cJSON_AddStringToObject(root, key, value);
                // BLINKER_LOG_ALL("cJSON_AddStringToObject");
            }
        }

        char *_data = cJSON_PrintUnformatted(root);
        strcpy(blinker_send_buf, _data);
        free(_data);
        cJSON_Delete(root);
        // BLINKER_LOG_ALL("auto_format_data2: %s", blinker_send_buf);
        // BLINKER_LOG_ALL("auto_format_data2 end");
    }
    else
    {
        cJSON *root = cJSON_CreateObject();

        if (isJson(value) && !isRaw)
        {
            cJSON *new_item = cJSON_Parse(value);
            cJSON_AddItemToObject(root, key, new_item);
        }
        else
        {
            if (isRaw) 
            {
                // cJSON_AddRawToObject(root, key, value);
                
                cJSON_AddItemToObject(root, key, cJSON_CreateIntArray(raw_data, raw_len));
                // BLINKER_LOG_ALL("cJSON_AddRawToObject");
            }
            else 
            {
                cJSON_AddStringToObject(root, key, value);
                // BLINKER_LOG_ALL("cJSON_AddStringToObject");
            }
        }

        char *_data = cJSON_PrintUnformatted(root);
        strcpy(blinker_send_buf, _data);
        free(_data);
        cJSON_Delete(root);
        // BLINKER_LOG_ALL("auto_format_data3: %s", blinker_send_buf);
        // BLINKER_LOG_ALL("auto_format_data3 end");
    }

    // BLINKER_LOG_ALL("auto_format_data end");
}

void blinker_print(const char * key, const char * value, int8_t isRaw)
{
    blinker_check_format();
    blinker_auto_format_data(key, value, isRaw);

    // if ((blinker_millis() - blinker_auto_format_time) >= BLINKER_MSG_AUTOFORMAT_TIMEOUT)
    // {
    //     blinker_auto_format_time = blinker_millis();
    // }

    // // BLINKER_LOG_ALL("blinker_auto_format_data end");

    uint8_t need_check = true;
    if (strstr(blinker_send_buf, BLINKER_CMD_CONNECTED)) need_check = false;
    strcat(blinker_send_buf, "\r\n");
    blinker_ble_print(blinker_send_buf, need_check);

    // free(blinker_send_buf);
    blinker_auto_format = false;
}

void blinker_heart_beat(cJSON *_data)
{
    cJSON *_state = cJSON_GetObjectItem(_data, BLINKER_CMD_GET);
    // cJSON *_state = cJSON_GetObjectItem(_data, BLINKER_CMD_GET);

    if (_state->string != NULL)
    {
        if (strcmp(_state->valuestring, BLINKER_CMD_STATE) == 0)
        {
            // char data[512] = "{\"state\":\"online\"}";
            
            // blinker_print(data);
            // blinker_print(BLINKER_CMD_STATE, BLINKER_CMD_CONNECTED, 0);

            blinker_parsed = true;
        }
    }

    // cJSON_Delete(_state);
}

void blinker_parse(const char * data)
{
    // BLINKER_LOG("blinekr parse data:%s", data);

    blinker_parsed = false;
    
    if (!isJson(data))
    {
        // BLINKER_ERR_LOG("not a json format data");
    }
    else
    {
        cJSON *root = cJSON_Parse(data);
        cJSON *_data = cJSON_GetObjectItem(root, "data");
        // cJSON *_data = cJSON_GetObjectItem(root, "data");

        if (_data == NULL)
        {
            blinker_heart_beat(root);
            widget_parse(root);

            if (!blinker_parsed)
            {
                if (blinker_data_func)
                {
                    blinker_data_func(root);

                    // cJSON_Delete(root);
                }
            }

    //         // cJSON_Delete(root);
        }
        else
        {
            // BLINKER_LOG("blinekr parse _data:%s", _data->valuestring);
            blinker_heart_beat(root);
            widget_parse(_data);

            if (!blinker_parsed)
            {
                if (blinker_data_func)
                {
                    blinker_data_func(root);

                    // cJSON_Delete(root);
                }
            }
        }

        // BLINKER_LOG_ALL("blinker_parse");

        // cJSON_Delete(_data);
        cJSON_Delete(root);
    }
    
    blinker_ble_flush();
}

void blinker_init(void)
{
    // // BLINKER_LOG_init();
    // // BLINKER_LOG("blinker v%s", BLINKER_VERSION);
    // // BLINKER_LOG("    To better use blinker with your IoT project!");
    // // BLINKER_LOG("    Download latest blinker library here!");
    // // BLINKER_LOG("    => https://github.com/blinker-iot/blinker-nRF52");        
    // // BLINKER_LOG("heap: %d", xPortGetFreeHeapSize());

    // run();

    blinker_ble_init(blinker_parse);
        
    // // BLINKER_LOG("heap: %d", xPortGetFreeHeapSize());
    
    // const char *strings[7]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
	// cJSON *root;
	// int numbers[3]={0,0,1};
	// root=cJSON_CreateObject();
    // // cJSON_CreateIntArray(numbers[i],3)
    // // for (uint8_t i=0;i<3;i++) 
	// cJSON_AddNumberToObject(root,"Height",600);
    // cJSON_AddItemToObject(root,"IDs", cJSON_CreateIntArray(numbers,3));
    // char *out=cJSON_Print(root);
    // cJSON_Delete(root);
    // printf("%s\n",out);
    // free(out);

    // vTaskStartScheduler();

    // // printf("run time %d,%d\r\n", app_timer_cnt_get(), app_timer_cnt_get()* ( (1 + 1 ) * 1000 ) / APP_TIMER_CLOCK_FREQ);
    // // // BLINKER_LOG("test");

    // for (;;)
    // {
    //     // idle_state_handle();
    //     APP_ERROR_HANDLER(NRF_ERROR_FORBIDDEN);
    // }
}

// void blinker_run(void * pv)
// {
//     // blinker_check_auto_format();

//     UNUSED_PARAMETER(pv);

//     while (1)
//     {
//         if (blinker_ble_avaliable())
//         {
//             blinker_parse(blinker_ble_lastread());
//             // BLINKER_LOG("heap: %d", xPortGetFreeHeapSize());
//             // blinker_ble_flush();
//         }
//         vTaskDelay(10);
//     }

//     // idle_state_handle();
// }

void run(void)
{
    // BLINKER_LOG("blinker run");

    // UNUSED_VARIABLE(xTaskCreate(blinker_run, "blinker_run", 128, NULL, 2, &m_blinkr_run_thread));
}
