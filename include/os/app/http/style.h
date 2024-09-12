#ifndef STYLE_H
#define STYLE_H


const char* style_css = " \
<style>\
body {\
    display: flex;\
    justify-content: center;\
    align-items: flex-start;\
    height: 100vh;\
    margin: 0;\
    background: #f0f8ff;\
    font-family: -apple-system, BlinkMacSystemFont, \"Segoe UI\", Roboto, \"Helvetica Neue\", Arial, sans-serif;\
    overflow-y: auto;\
    padding-top: 20px;\
    opacity: 0;\
    animation: fadeInBody 1s forwards;\
}\
.wrapper {\
    display: flex;\
    flex-direction: column;\
    align-items: center;\
    width: 100%;\
}\
.container {\
    display: flex;\
    flex-direction: column;\
    padding: 20px;\
    background: #fff;\
    border-radius: 15px;\
    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);\
    margin-bottom: 20px;\
    width: 300px;\
    opacity: 0;\
    transform: translateY(20px);\
    animation: fadeInSlide 1s forwards;\
}\
.header {\
    font-size: 18px;\
    font-weight: 500;\
    margin-bottom: 20px;\
    text-align: center;\
}\
.header2{\
    font-size: 15px;\
    font-weight: 400;\
    margin-bottom: 5px;\
    text-align: center;\
}\
label {\
    margin: 10px 0;\
    font-size: 14px;\
    display: flex;\
    justify-content: space-between;\
    align-items: center;\
}\
input[type=\"text\"], select {\
    padding: 10px;\
    margin-top: 5px;\
    border: 1px solid #ccc;\
    border-radius: 25px;\
    background: linear-gradient(135deg, #f0faff, #d0eaff);\
    font-size: 14px;\
    transition: background 0.3s;\
    font-family: inherit;\
    width: 200px;\
}\
input[type=\"text\"]:focus, select:focus {\
    background: linear-gradient(135deg, #e0f7ff, #c0e3ff);\
    outline: none;\
}\
.button-link, .button {\
    margin-top: 20px;\
    padding: 10px 20px;\
    background: linear-gradient(135deg, #6ec1e4, #3a8bd6);\
    border: none;\
    border-radius: 25px;\
    color: white;\
    text-align: center;\
    text-decoration: none;\
    font-size: 16px;\
    cursor: pointer;\
    transition: background 0.3s;\
    font-family: inherit;\
}\
.button-link:hover, .button:hover {\
    background: linear-gradient(135deg, #5aa9d4, #337bbf);\
}\
input[type=\"range\"] {\
    -webkit-appearance: none;\
    width: 100%;\
    height: 15px;\
    border-radius: 25px;\
    background: linear-gradient(135deg, #f0faff, #d0eaff);\
    outline: none;\
    transition: background 0.3s;\
}\
input[type=\"range\"]::-webkit-slider-thumb {\
    -webkit-appearance: none;\
    appearance: none;\
    width: 25px;\
    height: 25px;\
    border-radius: 50%;\
    background: linear-gradient(135deg, #6ec1e4, #3a8bd6);\
    cursor: pointer;\
    transition: background 0.3s;\
}\
input[type=\"range\"]::-moz-range-thumb {\
    width: 25px;\
    height: 25px;\
    border-radius: 50%;\
    background: linear-gradient(135deg, #6ec1e4, #3a8bd6);\
    cursor: pointer;\
    transition: background 0.3s;\
}\
input[type=\"range\"]:focus {\
    background: linear-gradient(135deg, #e0f7ff, #c0e3ff);\
}\
@keyframes fadeInBody {\
    from { opacity: 0; }\
    to { opacity: 1; }\
}\
@keyframes fadeInSlide {\
    from {\
        opacity: 0;\
        transform: translateY(20px);\
    }\
    to {\
        opacity: 1;\
        transform: translateY(0);\
    }\
}\
.add-button-wrapper {\
    display: flex;\
    justify-content: center;\
    width: 100%;\
    margin-bottom: 20px;\
}\
.add-button {\
    display: flex;\
    justify-content: center;\
    align-items: center;\
    width: 50px;\
    height: 50px;\
    background: linear-gradient(135deg, #6ec1e4, #3a8bd6);\
    border: none;\
    border-radius: 50%;\
    color: white;\
    font-size: 24px;\
    cursor: pointer;\
    transition: background 0.3s;\
    font-family: inherit;\
}\
.add-button:hover {\
    background: linear-gradient(135deg, #5aa9d4, #337bbf);\
}\
.plus-icon {\
    display: block;\
    width: 20px;\
    height: 20px;\
    position: relative;\
}\
.plus-icon::before,\
.plus-icon::after {\
    content: '';\
    position: absolute;\
    background-color: white;\
}\
.plus-icon::before {\
    width: 2px;\
    height: 100%;\
    left: 50%;\
    top: 0;\
    transform: translateX(-50%);\
}\
.plus-icon::after {\
    width: 100%;\
    height: 2px;\
    top: 50%;\
    left: 0;\
    transform: translateY(-50%);\
}\
</style>\
";

#endif