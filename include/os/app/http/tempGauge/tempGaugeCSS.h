#ifndef TEMP_GAUGE_CSS_H
#define TEMP_GAUGE_CSS_H

/**
 * @file include/os/app/http/tempGauge/tempGaugeCSS.h
 * @brief Embedded web UI asset used by the Home Control Station HTTP interface.
 */



const char* tempGaugeCSS = "\
<style>\
        .temperature-container {\
            display: flex;\
            justify-content: center;\
            align-items: center;\
            gap:12px;\
            width:100%;\
            min-width:0;\
            padding: 2px 0;\
            margin: 0 auto;\
            flex-wrap: nowrap;\
        }\
        .temperature-widget, .humidity-widget  {\
            flex:1 1 0;\
            min-width:0;\
            text-align: center;\
            color:#76A8FF;\
            position: relative;\
        }\
        .temperature-widget canvas, .humidity-widget canvas {\
            width: min(120px,100%);\
            height: 104px;\
        }\
        .temperature-value, .value-display{\
            font-size: 20px;\
            font-family:inherit;\
            font-weight:600;\
            color: #D5E2F8;\
            text-shadow:none;\
        }\
        .value-display{\
            font-size: 20px;\
            font-family:inherit;\
            color:#D5E2F8;\
            text-shadow:none;\
        }\
    </style>\
";


#endif