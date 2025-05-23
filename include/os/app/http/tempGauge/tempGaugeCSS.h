#ifndef TEMP_GAUGE_CSS_H
#define TEMP_GAUGE_CSS_H


const char* tempGaugeCSS = "\
<style>\
        .temperature-container {\
            display: flex;\
            justify-content: space-around;\
            align-items: center;\
            padding: 0px;\
            border-radius: 10px;\
            margin: 0px auto;\
            flex-wrap: wrap;\
        }\
        .temperature-widget, .humidity-widget  {\
            text-align: center;\
            color:#5A8DEE;\
            position: relative;\
        }\
        .temperature-widget canvas, .humidity-widget canvas {\
            width: 120px;\
            height: 110px;\
        }\
        .temperature-value, .value-display{\
            font-size: 21px;\
            font-family: Arial, sans-serif;\
            color: #b2cdff;\
            text-shadow:0 4px 12px rgba(0,0,0,0.3);\
        }\
        .value-display{\
            font-size: 21px;\
            font-family: Arial, sans-serif;\
            color:#b2cdff;\
            text-shadow:0 4px 12px rgba(0,0,0,0.3);\
        }\
    </style>\
";


#endif