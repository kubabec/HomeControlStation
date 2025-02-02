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
            margin: 15px;\
            position: relative;\
        }\
        .temperature-widget canvas, .humidity-widget canvas {\
            width: 120px;\
            height: 110px;\
        }\
        .temperature-value, .value-display{\
            font-size: 24px;\
            font-family: Arial, sans-serif;\
            color: #e74c3c;\
            margin-top: -15px;\
            font-weight: bold;\
        }\
        .value-display{\
            font-size: 24px;\
            font-family: Arial, sans-serif;\
            color:rgb(32, 184, 226);\
            margin-top: 5px;\
            font-weight: bold;\
        }\
    </style>\
";


#endif